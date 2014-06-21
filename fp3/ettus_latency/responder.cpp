//
// Copyright 2010-2011 Ettus Research LLC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <curses.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <complex>
#include <csignal>
#include <map>

#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/utils/msg.hpp>

using namespace std;

namespace po = boost::program_options;

static WINDOW* window = NULL;

typedef struct Stats
{
    double delay;
    uint64_t detected;
    uint64_t missed;
    uint64_t skipped;
} STATS;

typedef std::map<uint64_t,STATS> StatsMap;
/*
typedef struct TestState
{
    double delay_current;
    double delay_max, delay_min, delay_step;
} TEST_STATE;
*/
static bool stop_signal_called = false;

static void sig_int_handler(int signal)
{
    stop_signal_called = true;
}
/*
inline string replace(const string& _str, const string& a, const string& b) // boost::replace_all
{
    string str(_str);
    size_t n = 0;
    while ((n = str.find(a, n)) != string::npos)
    {
        str.replace(n, a.length(), b);
        n += b.length();
    }
    return str;
}
*/
#define FLUSH_SCREEN()      printw(ss.str().c_str()); refresh(); ss.str("")
#define FLUSH_SCREEN_NL()   \
    do { int y, x; \
    getyx(window, y, x); \
    if (x > 0) printw("\n"); \
    FLUSH_SCREEN(); } while (0)

static int _late_count = 0;

static void _late_handler(uhd::msg::type_t type, const std::string& msg)
{
    if (msg == "L") // This is just a test
    {
        ++_late_count;
        //printw("l");
        //refresh();
    }
}

static void _screen_handler(uhd::msg::type_t type, const std::string& msg)
{
    printw(msg.c_str());
    printw("\n");
    refresh();
}

int UHD_SAFE_MAIN(int argc, char *argv[])
{
    std::stringstream ss;
    std::ostream& _cout = cout;
    std::ostream& _cerr = cerr;
#define cout ss
#define cerr ss
    if (uhd::set_thread_priority_safe(1.0, true) == false)
      cerr << "Failed to set real-time" << endl;

    //variables to be set by po
    std::string device_args;
    double delay;
    //size_t total_num_samps;
    double sample_rate;
    double rx_center_freq; //CSG
    double tx_center_freq; //CSG
    double rx_gain; //CSG
    double tx_gain; //CSG
    double rx_bandwidth; //CSG
    double tx_bandwidth; //CSG

    double trigger_level;
    float output_scale;
    double response_duration;
    size_t samps_per_buff, samps_per_packet;
    double dc_offset_delay;
    double init_delay;
    double timeout;
    double level_calibration_duration;
    string stats_filename;
    //size_t iterations_per_delay;
    double delay_max, delay_min, delay_step;
    double pulse_detection_threshold;
    //TEST_STATE stateTest;
    uint64_t test_iterations;
    size_t end_test_after_success_count;
    size_t skip_iterations; // Remove false negative in first delay test
    double simulate_frequency;
    double time_mul;
    string stats_filename_prefix, stats_filename_suffix;
    size_t flush_count;
    size_t optimize_padding;

    ////////////////////////////////////////
    //setup the program options
    ////////////////////////////////////////
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "help message")
        ("args", po::value<std::string>(&device_args)->default_value(""), "single uhd device address args")
        ("delay", po::value<double>(&delay)->default_value(0.005), "number of seconds in the future to reply")
        //("nsamps", po::value<size_t>(&total_num_samps)->default_value(10000), "total number of samples to transmit")
        ("rate", po::value<double>(&sample_rate)->default_value(1e6), "rate of outgoing samples")
        ("rx-freq", po::value<double>(&rx_center_freq)->default_value(300e6), "Center frequency of RX channel")
        ("tx-freq", po::value<double>(&tx_center_freq)->default_value(250e6), "Center frequency of TX channel")
        ("rx-gain", po::value<double>(&rx_gain)->default_value(10), "Gain of RX channel")
        ("tx-gain", po::value<double>(&tx_gain)->default_value(1), "Gain of TX channel")
        ("rx-band", po::value<double>(&rx_bandwidth)->default_value(1e6/2), "Bandwidth of RX channel")
        ("tx-band", po::value<double>(&tx_bandwidth)->default_value(1e6/2), "Bandwidth of TX channel")
        ("level", po::value<double>(&trigger_level)->default_value(0.5), "trigger level as fraction of high level")
        ("scale", po::value<float>(&output_scale)->default_value(float(0.3)), "output scaling")
        ("duration", po::value<double>(&response_duration)->default_value(0.01), "duration of response (seconds)")
        ("dc-offset-delay", po::value<double>(&dc_offset_delay)->default_value(0), "duration of DC offset calibration (seconds)")   // This stage is not necessary
        ("init-delay", po::value<double>(&init_delay)->default_value(0.5), "initialisation delay (seconds)")
        ("timeout", po::value<double>(&timeout)->default_value(3.0), "stream timeout (seconds)")
        ("spb", po::value<size_t>(&samps_per_buff)->default_value(1024), "samples per buffer")
        ("spp", po::value<size_t>(&samps_per_packet)->default_value(0), "samples per packet (0: use default)")
        ("calib", po::value<double>(&level_calibration_duration)->default_value(0.5), "level calibration duration (seconds)")
        ("invert", "input signal inversion")
        ("invert-output", "output signal inversion")
        ("no-delay", "disable timed delay")
        ("allow-late", "allow late bursts")
        ("stats-file", po::value<std::string>(&stats_filename)->default_value(""), "test statistics output filename (empty: auto-generate)")
        //("ipd", po::value<size_t>(&iterations_per_delay)->default_value(1000), "iterations per delay")
        ("delay-min", po::value<double>(&delay_min)->default_value(0.0001), "minimum delay")
        ("delay-max", po::value<double>(&delay_max)->default_value(0.005), "maximum delay")
        ("delay-step", po::value<double>(&delay_step)->default_value(0.000001), "delay step")
        ("pdt", po::value<double>(&pulse_detection_threshold)->default_value(1e-3), "pulse detection threshold")
        ("iterations", po::value<uint64_t>(&test_iterations)->default_value(0), "test iterations")
        ("test-duration", "treat test iterations as duration")
        ("test-success", po::value<size_t>(&end_test_after_success_count)->default_value(0), "end test after multiple successful delays (0: run through all delays)")
        ("skip-iterations", po::value<size_t>(&skip_iterations)->default_value(50), "skip first iterations for each delay")
        ("simulate", po::value<double>(&simulate_frequency)->default_value(0.0), "frequency of simulation event (Hz)")
        ("time-mul", po::value<double>(&time_mul)->default_value(1.0), "statistics output time multiplier")
        ("ignore-simulation-check", "ignore if simulation rate exceeds maximum delay + response duration")
        ("stats-file-prefix", po::value<std::string>(&stats_filename_prefix)->default_value(""), "test statistics output filename prefix")
        ("stats-file-suffix", po::value<std::string>(&stats_filename_suffix)->default_value(""), "test statistics output filename suffix")
        ("flush", po::value<size_t>(&flush_count)->default_value(16), "number of zero samples to add to a burst to flush hardware")
        ("no-eob", "disable end-of-burst")
        ("adjust-simulation-rate", "adjust simulation rate if it will be too fast for maximum delay duration")
        ("optimize-simulation-rate", "make simulation rate as fast as possible for each delay")
        ("optimize-padding", po::value<size_t>(&optimize_padding)->default_value(16), "time (as number of samples) to pad optimized simulation rate")
        ("no-stats-file", "do not output statistics file")
        ("batch-mode", "disable user prompts")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);


    ////////////////////////////////////////
    //Print Help Message
    ////////////////////////////////////////
    if (vm.count("help"))
    {
        _cout << boost::format("UHD Latency Test %s") % desc << std::endl;
        return 0;
    }


    ////////////////////////////////////////
    //Set up test variables
    ////////////////////////////////////////
    delay_step = abs(delay_step);
    if (delay_min > delay_max)
        delay_step *= -1;

    uint64_t response_length = (uint64_t)(sample_rate * response_duration);
    int64_t init_delay_count = (int64_t)(sample_rate * init_delay);
    int64_t dc_offset_countdown = (int64_t)(sample_rate * dc_offset_delay);
    uint64_t level_calibration_count = (uint64_t)(sample_rate * level_calibration_duration);
    int64_t level_calibration_countdown = (int64_t)level_calibration_count;
    bool no_delay = (vm.count("no-delay") > 0);
    bool adjust_simulation_rate = (vm.count("adjust-simulation-rate") > 0);
    double original_simulate_frequency = simulate_frequency;
    bool optimize_simulation_rate = (vm.count("optimize-simulation-rate") > 0);
    bool no_stats_file = (vm.count("no-stats-file") > 0);
    bool batch_mode = (vm.count("batch-mode") > 0);

    uint64_t simulate_duration = 0, original_simulate_duration = 0;
    uint64_t simulate_duration_cnt =0;

    //////////////////////
    //Simulation Setup 
    //////////////////////
    if (simulate_frequency > 0.0)
    {
        original_simulate_duration = simulate_duration = (uint64_t)((double)sample_rate / simulate_frequency);

        if (simulate_duration > 0)
        {
            double highest_delay = 0.0;

            if (test_iterations > 0)
                highest_delay = max(delay_max, delay_min);
            else if (no_delay == false)
                highest_delay = delay;

            uint64_t highest_delay_samples = (uint64_t)(highest_delay * (double)sample_rate);
            if ((highest_delay_samples + response_length + flush_count) > simulate_duration)
            {
                int max_possible_rate = (int)((double)sample_rate / (double)(highest_delay_samples + response_length + flush_count));
                double max_possible_delay = (double)(simulate_duration - (response_length + flush_count)) / (double)sample_rate;

                if (adjust_simulation_rate == false)
                    _cerr << boost::format("Highest delay and response duration will exceed the pulse simulation rate (%ld + %ld > %ld samples)") % highest_delay_samples % response_length % simulate_duration << endl;

                if (adjust_simulation_rate) // This is now done DURING the simulation based on active delay
                {
                    //simulate_frequency = max_possible_rate;
                    //simulate_duration = (uint64_t)((double)sample_rate / simulate_frequency);
                }
                else
                {
                    _cerr << boost::format("Simulation rate must be less than %i Hz, or maximum delay must be less than %f s") % max_possible_rate % max_possible_delay << endl;

                    if (vm.count("ignore-simulation-check") == 0)
                        return -1;
                }
            }

            // Skip settling period and calibration
            init_delay_count = 0;
            dc_offset_countdown = 0;
            level_calibration_countdown = 0;
        }
        else
        {
            _cerr << boost::format("Simulation frequency too high (%f Hz)") % simulate_frequency << endl;
            return -1;
        }
    }

   ///////////////////////////////////
   //Screen Handler
   ///////////////////////////////////
   
    uhd::msg::register_handler(&_screen_handler);

   ///////////////////////////////////
   //Setup User break
   ///////////////////////////////////
    window = initscr();

    cbreak();       // Unbuffered key input, except for signals (cf. 'raw')
    noecho();

    nonl();
    intrflush(window, FALSE);
    keypad(window, TRUE);   // Enable function keys, arrow keys, ...

    nodelay(window, 0);

    timeout(0);

    ///////////////////////////////////
    //Create USRP device
    ///////////////////////////////////
    uint64_t num_total_samps = 0, num_total_samps_test = 0;
    uhd::usrp::multi_usrp::sptr usrp;

    StatsMap mapStats;
    string test_id;

    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);

    int return_code = 0;
    size_t overruns = 0;

    //Start of Try Data
    try {    

    //set some test parameters and set the output response
    bool allow_late_bursts = (vm.count("allow-late") > 0);
    bool test_iterations_is_sample_count = (vm.count("test-duration") > 0);
    float invert = ((vm.count("invert") > 0) ? -1.0f : 1.0f);
    float output_value = ((vm.count("invert-output") > 0) ? -1.0f : 1.0f);
    bool skip_eob = (vm.count("no-eob") > 0);

    int success_count = 0;

    // Force certain settings during test mode
    if (test_iterations > 0)    
    {
        no_delay = false;
        allow_late_bursts = false;
        delay = delay_min;
    }

    //create a usrp device
    cout << "Creating the USRP device";
    if (device_args.empty() == false)
        cout << " with args \"" << device_args << "\"";
    cout << "..." << endl;
    FLUSH_SCREEN();
    usrp = uhd::usrp::multi_usrp::make(device_args);
    FLUSH_SCREEN();
    erase();
    refresh();
    cout << boost::format("Using device:\n%s") % usrp->get_pp_string() << std::endl;
    test_id = usrp->get_mboard_name();

    //set the rx sample rate
    cout << boost::format("Setting RX rate: %f Msps...") % (sample_rate/1e6) << std::endl;
    usrp->set_rx_rate(sample_rate);
    cout << boost::format("Actual RX rate:  %f Msps") % (usrp->get_rx_rate()/1e6) << std::endl;

    //////CSG Additions

    //CSG RX Set Center Freq
    cout << boost::format("Setting RX Center frequency: %f Msps...") % (rx_center_freq/1e6) << std::endl;
    usrp->set_rx_freq(rx_center_freq, 0);
    cout << boost::format("Actual RX Center frequency:  %f Msps") % (usrp->get_rx_freq()/1e6) << std::endl;

    //CSG Set RX Gain
    cout << boost::format("Setting RX Gain: %f dB...") % (rx_gain) << std::endl;
    usrp->set_rx_gain (rx_gain,0);
    cout << boost::format("Actual RX Gain:  %f dB") % (usrp->get_rx_gain()) << std::endl;

    //CSG Set RX Bandwidth
    cout << boost::format("Setting RX Bandwidth: %f MHz...") % (rx_bandwidth/1e6) << std::endl;
    usrp->set_rx_bandwidth (rx_bandwidth,0);
    cout << boost::format("Actual RX Bandwidth:  %f MHz") % (usrp->get_rx_bandwidth()/1e6) << std::endl;		

    //CSG Set TX Center Freq
    cout << boost::format("Setting TX Center frequency: %f Msps...") % (tx_center_freq/1e6) << std::endl;
    usrp->set_tx_freq(tx_center_freq, 0);
    cout << boost::format("Actual TX Center frequency:  %f Msps") % (usrp->get_tx_freq()/1e6) << std::endl;

    //CSG Set TX Gain
    cout << boost::format("Setting TX Gain: %f dB...") % (tx_gain) << std::endl;
    usrp->set_tx_gain (tx_gain,0);
    cout << boost::format("Actual TX Gain:  %f dB") % (usrp->get_tx_gain()) << std::endl;

    //CSG Set TX Bandwidth
    cout << boost::format("Setting TX Bandwidth: %f MHz...") % (tx_bandwidth/1e6) << std::endl;
    usrp->set_tx_bandwidth (tx_bandwidth,0);
    cout << boost::format("Actual TX Bandwidth:  %f MHz") % (usrp->get_tx_bandwidth()/1e6) << std::endl;		



    //set the tx sample rate
    cout << boost::format("Setting TX rate: %f Msps...") % (sample_rate/1e6) << std::endl;
    usrp->set_tx_rate(sample_rate);
    cout << boost::format("Actual TX rate:  %f Msps") % (usrp->get_tx_rate()/1e6) << std::endl;

    //device timestamp
    //cout << boost::format("Setting device timestamp to 0...") << std::endl;
    //usrp->set_time_now(uhd::time_spec_t(0.0));

    //Set Samples per Buffer
    cout << std::endl;
    cout << boost::format("Samples per buffer: %d") % samps_per_buff << endl;

    /////////////////////////////////
    //create a Receive streamer
    /////////////////////////////////
    uhd::stream_args_t stream_args("fc32"); //complex floats
    if (samps_per_packet > 0)
    {
        cout << boost::format("Samples per packet set to: %d") % samps_per_packet << endl;
        stream_args.args["spp"] = str(boost::format("%d") % samps_per_packet);
    }
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    samps_per_packet = rx_stream->get_max_num_samps();

    cout << boost::format("Flushing burst with %d samples") % flush_count << endl;

    if (skip_eob)
        cout << "Skipping End-Of-Burst" << endl;

    /////////////////////////////////
    //create a transmit streamer
    /////////////////////////////////
    uhd::stream_args_t tx_stream_args("fc32"); //complex floats
    if (allow_late_bursts == false)
    {
        cout << "Underflow policy set to drop late bursts" << endl;
        tx_stream_args.args["underflow_policy"] = "next_burst";
    }
    else
        cout << "Underflow policy set to allow late bursts" << endl;
    uhd::tx_streamer::sptr tx_stream = usrp->get_tx_stream(tx_stream_args);

    cout << boost::format("Maximum number of samples: RX = %d, TX = %d") % rx_stream->get_max_num_samps() % tx_stream->get_max_num_samps() << endl;

    //allocate buffer with data to send
    cout << boost::format("Response length: %ld samples (%f us)") % response_length % (response_duration * 1e6) << endl;

    // Flush the half-band filters on the hardware
    float* pResponse = new float[(response_length + flush_count) * 2];
    for (int i = 0; i < (response_length * 2); ++i)
        pResponse[i] = output_value * output_scale;
    for (int i = (response_length * 2); i < ((response_length + flush_count) * 2); ++i)
        pResponse[i] = 0.0f;

    cout << std::endl;

    if (simulate_duration > 0)
        cout << boost::format("Simulating pulses at %f Hz (every %ld samples)") % simulate_frequency % simulate_duration << endl;

    ///////////////////////////////////////////////////////////////////////////
    //Setup Test
    ///////////////////////////////////////////////////////////////////////////
    STATS statsCurrent, statsPrev;
    memset(&statsCurrent, 0x00, sizeof(STATS));

    if (test_iterations > 0)
    {
        cout << boost::format("Test coverage: %f -> %f (%f steps)") % delay_min % delay_max % delay_step << endl;

        if (end_test_after_success_count > 0)
            cout << boost::format("Testing will end after %d successful delays") % end_test_after_success_count << endl;

        statsCurrent.delay = delay_min;
    }

 
    std::signal(SIGINT, &sig_int_handler);
    if (test_iterations > 0)
        cout << "Press Ctrl + C to abort test" << std::endl << endl;
    else
        cout << "Press Q stop streaming" << std::endl << endl;

    std::vector<std::complex<float> > buff(samps_per_buff);
    uhd::rx_metadata_t rx_md;
    bool overflow_message = true;

    if ((dc_offset_countdown == 0) && (simulate_frequency == 0.0))
    {
        usrp->set_rx_dc_offset(false);
        cout << "DC offset disabled" << endl;
    }

    double ave_high = 0, ave_low = 0;
    int ave_high_count = 0, ave_low_count = 0;
    bool level_calibration_stage_2 = false;
    float threshold = 0;

    bool input_state = false;
    uint64_t trigger_count = 0;

    uhd::async_metadata_t async_md;

    FLUSH_SCREEN();

    ///////////////////////////////////////////////////////////////////////////
    //Setup memory and start streaming
    ///////////////////////////////////////////////////////////////////////////

    uhd::msg::register_handler(&_late_handler);

    //setup streaming
    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS;
    //stream_cmd.num_samps = num_requested_samples;
    stream_cmd.stream_now = true;
    //stream_cmd.time_spec = uhd::time_spec_t();
    usrp->issue_stream_cmd(stream_cmd);

    int late_count = 0;
    rx_md.has_time_spec = true;
    //look at first one
    /*size_t num_rx_samps = rx_stream->recv(&buff.front(), buff.size(), rx_md, timeout);
    cout << boost::format("RX first buffer size %f") % num_rx_samps << std::endl;
    cout <<  boost::format("RX first buffer Has time spec?: %s") % rx_md.has_time_spec << std::endl;
    cout <<  boost::format("RX first buffer time spec %s") % rx_md.time_spec << std::endl;
    */

    //////////////////
    //start streaming
    //////////////////
    while (not stop_signal_called/* and (num_requested_samples != num_total_samps or num_requested_samples == 0)*/) {
        //////////////////
        //Get Next packet, report overruns later
        //////////////////
        size_t num_rx_samps = rx_stream->recv(&buff.front(), buff.size(), rx_md, timeout);


        //////////////////
        //Check for errors
        //////////////////
        if (rx_md.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT)
        {
            cout << boost::format("Timeout while streaming") << std::endl;
            break;
        }
        else if (num_rx_samps </*=*/ 0)
        {
            cout << boost::format("rx_stream->recv returned %d") % num_rx_samps << std::endl;
            break;
        }
        else if ((num_rx_samps == 0) && (rx_md.error_code == uhd::rx_metadata_t::ERROR_CODE_NONE))
        {
            cout << "Received no samples" << endl;
            //continue;
            break;
        }
        else if (rx_md.error_code == uhd::rx_metadata_t::ERROR_CODE_OVERFLOW)
        {
            ++overruns;
            /*if (overflow_message) {
                overflow_message = false;
                std::cerr << boost::format(
                    "Got an overflow indication. Please consider the following:\n"
                    "  Your write medium must sustain a rate of %fMB/s.\n"
                    "  Dropped samples will not be written to the file.\n"
                    "  Please modify this example for your purposes.\n"
                    "  This message will not appear again.\n"
                ) % (usrp->get_rx_rate()*sizeof(std::complex<float>)/1e6);
            }*/
            //continue;
            //printw("o"); refresh();
        }
        else if (rx_md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE)
        {
            throw std::runtime_error(str(boost::format(
                "Unexpected error code 0x%x"
            ) % rx_md.error_code));
        }
        
        ////////////////////////////////////////////////////////////
        //Calibration
        ////////////////////////////////////////////////////////////
        if (dc_offset_countdown > 0)
        {
            dc_offset_countdown -= (int64_t)num_rx_samps;
            if (dc_offset_countdown > 0)
                continue;
            usrp->set_rx_dc_offset(false);
            cout << "DC offset calibration complete" << endl;
            FLUSH_SCREEN();
        }

        if (init_delay_count > 0)
        {
            init_delay_count -= (int64_t)num_rx_samps;
            if (init_delay_count > 0)
                continue;
            cout << "Initial settling period elapsed" << endl;
            FLUSH_SCREEN();
        }

        if (level_calibration_countdown > 0)
        {
            if (level_calibration_stage_2 == false)
            {
                //const int window_size = 1000;
                float average = 0;
                for (size_t n = 0; n < num_rx_samps; n++)
                    average += buff[n].real() * invert;
                average /= (float)num_rx_samps;

                if (ave_low_count == 0)
                {
                    ave_low = average;
                    ++ave_low_count;
                }
                else if (average < ave_low)
                {
                    ave_low = average;
                    ++ave_low_count;
                }

                if (ave_high_count == 0)
                {
                    ave_high = average;
                    ++ave_high_count;
                }
                else if (average > ave_high)
                {
                    ave_high = average;
                    ++ave_high_count;
                }
            }
            else {
                for (size_t n = 0; n < num_rx_samps; n++)
                {
                    float f = buff[n].real() * invert;
                    if (f >= threshold)
                    {
                        ave_high += f;
                        ave_high_count++;
                    }
                    else
                    {
                        ave_low += f;
                        ave_low_count++;
                    }
                }
            }
            //cout << boost::format("CSG number RX Samples %d") % num_rx_samps<< endl;
            level_calibration_countdown -= (int64_t)num_rx_samps;

            if (level_calibration_countdown <= 0)
            {
                if (level_calibration_stage_2 == false)
                {
                    level_calibration_stage_2 = true;
                    level_calibration_countdown = level_calibration_count;
                    threshold = ave_low + ((ave_high - ave_low) / 2.0); //orginal
                    //threshold = trigger_level;
                    cout << boost::format("\n\r Phase #1: Ave low: %.3f (#%d), ave high: %.3f (#%d), threshold: %.3f") % ave_low % ave_low_count % ave_high % ave_high_count % threshold << endl;                                                          
                    cout << endl;
                    FLUSH_SCREEN();  
                    ave_low_count = ave_high_count = 0;
                    ave_low = ave_high = 0.0f;
                    continue;
                }
                else
                {
                    ave_low /= (double)ave_low_count;
                    ave_high /= (double)ave_high_count;
                    //threshold = ave_low + ((ave_high - ave_low) * trigger_level); //orginal
                    threshold = trigger_level;//CSG_2_19
                    cout << boost::format("Phase #2: Ave low: %.3f (#%d), ave high: %.3f (#%d), threshold: %.3f \n") % ave_low % ave_low_count % ave_high % ave_high_count % threshold << endl;
                    cout << endl;
                    FLUSH_SCREEN();
  
                    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
                    stream_cmd.stream_now = true;
                    usrp->issue_stream_cmd(stream_cmd);

                    double diff = abs(ave_high - ave_low);
                    if (diff < pulse_detection_threshold)
                    {
                        cout << boost::format("Did not detect any pulses (difference %.6f < detection threshold %.6f)") % diff % pulse_detection_threshold << endl;                        
                        break;
                    }//CSG_2_19
                    else
                    {
                        cout << boost::format("Detected pulses (difference %.6f > detection threshold %.6f)") % diff % pulse_detection_threshold << endl;                        
                        FLUSH_SCREEN_NL();
                    }

                    // Flush the buffers

                    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS;
                    stream_cmd.stream_now = true;
                    usrp->issue_stream_cmd(stream_cmd);
                }
            }
            else
                continue;
        }
        ////////////////////////////////////////////////////////////
        //Set times on first run of each new time delay
        ////////////////////////////////////////////////////////////
        if (num_total_samps == 0)
        {
            if (test_iterations > 0)
            {
                uint64_t old_simulate_duration = simulate_duration;
                
                uint64_t highest_delay_samples = (uint64_t)(delay * (double)sample_rate);
                int max_possible_rate = (int)((double)sample_rate / (double)((delay * (double)sample_rate) + response_length + flush_count + optimize_padding));
                if ((optimize_simulation_rate) ||
                    ((highest_delay_samples + response_length + flush_count) > original_simulate_duration))
                {
                   //CSG --Doesn't work with USRP2-- assert(optimize_simulation_rate || adjust_simulation_rate);

                    simulate_frequency = max_possible_rate;
                    //simulate_duration = (uint64_t)((double)sample_rate / simulate_frequency);
                    simulate_duration = (uint64_t)(( (double)sample_rate / simulate_frequency) *10);
                }

                cout << boost::format("Delay now: %.6f") % delay;    // Print this for the very first test run
                if (old_simulate_duration != simulate_duration)
                    cout << boost::format(" [Simulation rate now: %f Hz (%ld samples)]") % simulate_frequency % simulate_duration;
                FLUSH_SCREEN();
                
                simulate_duration_cnt = simulate_duration;        
            }
        }
        
        ////////////////////////////////////////////////////////////
        //For Each RX Buffer  "test for pulse and send"
        ////////////////////////////////////////////////////////////
        //save Rx samples file
        //outfile.write((const char*)&buff.front(), num_rx_samps*sizeof(std::complex<float>));
        size_t n = 0;
//org        for (size_t n = 0; n < num_rx_samps; n++)
        while  (n < num_rx_samps)
        {
            float f = buff[n].real() * invert;
            bool new_state = false;

//org            if (simulate_duration > 0) //org
//org              new_state = (((num_total_samps + n) % simulate_duration) == 0);//org
//org            else//org
//org              new_state = (f >= threshold);   
               if (simulate_duration_cnt > 0) //org
                   simulate_duration_cnt--;
               else//org                   
                   new_state = (f >= threshold);   

            //waits for last statement to be true and new state to be false (Negative Edge Detect)
            if ((new_state == false) && (input_state))
            {
                simulate_duration_cnt = simulate_duration;
                trigger_count++;
                statsCurrent.detected++;
                //handle setting the skip itterations flag
                if ((test_iterations > 0) && (skip_iterations > 0) && 
                    (statsCurrent.skipped == 0) && (skip_iterations == statsCurrent.detected))
                {
                    memset(&statsCurrent, 0x00, sizeof(STATS));
                    statsCurrent.delay = delay;
                    statsCurrent.detected = 1;
                    statsCurrent.skipped = skip_iterations;

                    trigger_count = 1;
                    late_count = 0;
                }

                //set UHD timing guidlines
                uhd::tx_metadata_t tx_md;
                tx_md.start_of_burst = true;
                tx_md.end_of_burst = false;
                if (no_delay == false)
                {
                    tx_md.has_time_spec = true;
                    tx_md.time_spec = rx_md.time_spec + uhd::time_spec_t(0, n, sample_rate) + uhd::time_spec_t(delay);
                    //cout << boost::format("TX time spec %.6f ") % tx_md.time_spec << endl;                        
                }
                else
                    tx_md.has_time_spec = false;

                //send a single packet
                const size_t length_to_send = response_length + (flush_count - 1);
                size_t num_tx_samps = tx_stream->send(pResponse, length_to_send, tx_md, timeout);

                //check for TX errors
                if (num_tx_samps < length_to_send)
                {
                    //cerr << "Send timeout Single Packet \n" << std::endl;
	            cout << boost::format("Send timeout Single Packet: Number tx samps sent %f, lenght to send %f\n") %  num_tx_samps % length_to_send << std::endl;
                    FLUSH_SCREEN_NL(); 
                }

                if (skip_eob == false)
                {
                    tx_md.start_of_burst = false;
                    tx_md.end_of_burst = true;
                    tx_md.has_time_spec = false;
                    const size_t eob_length_to_send = 1;
                    size_t eob_num_tx_samps = tx_stream->send(&pResponse[length_to_send], eob_length_to_send, tx_md);
                    if (eob_num_tx_samps < eob_length_to_send)
                    {
                        cout << boost::format("Send timeout Skip EOB: Number tx samps sent %f, lenght to send %f\n") %  num_tx_samps %eob_length_to_send<< std::endl;
                        FLUSH_SCREEN_NL();
                        size_t eob_num_tx_samps = tx_stream->send(&pResponse[length_to_send], eob_length_to_send, tx_md); //CSG
                    }
                }
                // Look for Event timer
                if (usrp->get_device()->recv_async_msg(async_md, 0))
                {
                    if (async_md.event_code == uhd::async_metadata_t::EVENT_CODE_TIME_ERROR)
                    {
                        ++late_count;
                        statsCurrent.missed++;
                        //printw("L");
                        //refresh();
                    }
                }
                n = num_rx_samps; //CSG, end on current buffer go to next
            }//End of If statement (if ((new_state == false) && (input_state)))
            else //CSG
               n++; //CSG

            input_state = new_state;
        } //End TX transmitt loop

        ////////////////////////////////////////////////////////////

        if (test_iterations == 0)
        {
            int c = wgetch(window);
            if (c > -1)
            {
                //cout << c << endl;
                //FLUSH_SCREEN();

                if ((c == KEY_DOWN) || (c == KEY_UP))
                {
                    double dMag = log10(delay_step);
                    int iMag = (int)floor(dMag);
                    //if ((int)dMag == iMag)
                    //    --iMag;
                    iMag += ((c == KEY_UP) ? 1 : -1);
                    delay_step = pow(10.0, iMag);
                    cout << "Step: " << delay_step;
                }

                if ((c == KEY_LEFT) || (c == KEY_RIGHT))
                {
                    double step = delay_step * ((c == KEY_RIGHT) ? 1 : -1);
                    if ((delay + step) >= 0.0)
                        delay += step;
                    cout << "Delay: " << delay;
                }

                if (c == 'd')
                {
                    no_delay = !no_delay;

                    if (no_delay)
                        cout << "Delay disabled (best effort)";
                    else
                        cout << "Delay: " << delay;
                }
                else if (c == 'q')
                {
                    break;
                }
                else if (c == 'l')
                {
                    allow_late_bursts = !allow_late_bursts;

                    if (allow_late_bursts)
                        cout << "Allowing late bursts";
                    else
                        cout << "Dropping late bursts";
                }

                if ((c == KEY_DOWN) || (c == KEY_UP) || (c == KEY_LEFT) || (c == KEY_RIGHT) ||
                    (c == 'd') || (c == 'l'))
                {
                    int y, x;
                    getyx(window, y, x);
                    if (x > 0)
                    {
                        move(y, 0);
                        clrtoeol();
                        //move(y, 0);
                    }

                    //deleteln();
                    FLUSH_SCREEN();
                }
            }
        }

        ////////////////////////////////////////////////////////////

        num_total_samps += num_rx_samps;
        num_total_samps_test += num_rx_samps;

        ////////////////////////////////////////////////////////////

        if (test_iterations > 0)
        {
            if (((test_iterations_is_sample_count == false) && (trigger_count >= test_iterations)) ||
                ((test_iterations_is_sample_count) && (num_total_samps_test > test_iterations)))
            {
                trigger_count = 0;
                num_total_samps_test = 0;

                uint64_t key = (uint64_t)(delay * 1e6);
                mapStats[key] = statsCurrent;

                statsPrev = statsCurrent;

                if (statsCurrent.missed == 0)
                {
                    ++success_count;

                    if (success_count == end_test_after_success_count)
                    {
                        cout << endl;
                        cout << boost::format("Test complete after %d successes.") % success_count << endl;
                        FLUSH_SCREEN();
                        break;
                    }
                }
                else
                    success_count = 0;

                if (((delay_min <= delay_max) && (delay >= delay_max)) ||
                    ((delay_min > delay_max) && (delay <= delay_max)))
                {
                    cout << endl;
                    cout << "Test complete." << endl;
                    FLUSH_SCREEN();
                    break;
                }

                delay += delay_step;

                /////////////////////////////////

                uint64_t old_simulate_duration = simulate_duration;
                uint64_t highest_delay_samples = (uint64_t)(delay * (double)sample_rate);
                int max_possible_rate = (int)((double)sample_rate / (double)((delay * (double)sample_rate) + response_length + flush_count + optimize_padding));
                if ((optimize_simulation_rate) ||
                    ((highest_delay_samples + response_length + flush_count) > original_simulate_duration))
                {
                    //CSG assert(optimize_simulation_rate || adjust_simulation_rate);

                    simulate_frequency = max_possible_rate;
                    //CSG simulate_duration = (uint64_t)((double)sample_rate / simulate_frequency);
                }

                /////////////////////////////////

                int y, x;
                getyx(window, y, x);
                move(y, 0);
                clrtoeol();
                double score = 0;
                if (statsPrev.detected > 0)
                    score = (double)(statsPrev.detected - statsPrev.missed) / (double)statsPrev.detected;
                cout << boost::format("Delay now: %.6f (previous delay %.6f scored %f%% [%ld / %ld]) \n") % delay % statsPrev.delay % score % (statsPrev.detected - statsPrev.missed) % statsPrev.detected;
                if (old_simulate_duration != simulate_duration)
                    cout << boost::format(" [Simulation rate now: %f Hz (%ld samples)] \n") % simulate_frequency % simulate_duration;
                FLUSH_SCREEN();

                memset(&statsCurrent, 0x00, sizeof(STATS));
                statsCurrent.delay = delay;
            }
        }
    } //Stop Streaming


    } //End of Try Data
    catch (const std::runtime_error& e)
    {
        cout << e.what() << endl;

        return_code = 1;
    }
    catch (...)
    {
        cout << "Unhandled exception" << endl;

        return_code = 2;
    }

    try
    {
        if (usrp)
        {
            stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
            stream_cmd.stream_now = true;
            usrp->issue_stream_cmd(stream_cmd);
        }
    }
    catch (...)
    {
        //
    }

    if (ss.str().length() > 0)
    {
        ss << "Press any key to end..." << endl;
        printw(ss.str().c_str()); refresh();
        if (batch_mode == false)
        {
            timeout(-1);
            getch();
        }
    }

    endwin();



#undef cout
#undef cerr
    //cout << endl;
    cout << boost::format("Received %ld samples") % num_total_samps;
    if (overruns > 0)
        cout << boost::format(" (%d overruns)") % overruns;
    cout << endl;

    if (stats_filename.empty())
    {
        string file_friendly_test_id(test_id);
        boost::replace_all(file_friendly_test_id, " ", "_");
        stats_filename = str(boost::format("%slatency-stats.id_%s_rate_%i-spb_%i-spp_%i%s.txt") % stats_filename_prefix % file_friendly_test_id % (int)sample_rate % samps_per_buff % samps_per_packet % stats_filename_suffix);
    }

    if ((test_iterations > 0) && (stats_filename.empty() == false) && (no_stats_file == false))
    {
        if (mapStats.empty())
        {
            cout << "No results to output (not writing statistics file)" << endl;
        }
        else
        {
            try
            {
                ofstream results(stats_filename.c_str());

                for (StatsMap::iterator it = mapStats.begin(); it != mapStats.end(); ++it)
                {
                    STATS& stats = it->second;
                    double d = 0.0;
                    if (stats.detected > 0)
                        d = 1.0 - ((double)stats.missed / (double)stats.detected);
                    cout << /*setw(10)*/"\t" << setprecision(6) << /*it->first*/stats.delay /*<< " " */<< /*setw(12)*/"\t\t" << setprecision(6) << d << endl;

                    results << (/*((double)it->first * time_mul) / 1e6*/stats.delay * time_mul) << " " << setprecision(6) << d << endl;
                }

                cout << "Statistics written to: " << stats_filename << endl;
            }
            catch (...)
            {
                cout << "Failed to write statistics to: " << stats_filename << endl;
            }
        }
    }

    return return_code;
}
