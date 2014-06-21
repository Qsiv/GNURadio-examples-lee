#!/usr/bin/sh
sudo ./responder --spb=64 --rate=4e6 --iterations=10000 --delay-min=100e-6 --delay-max=5000e-6 --delay-step=50e-6 --duration=10e-6 --time-mul=1e6 --test-success=3 --simulate=199
sudo ./responder --spb=64 --rate=8e6 --iterations=10000 --delay-min=100e-6 --delay-max=5000e-6 --delay-step=50e-6 --duration=10e-6 --time-mul=1e6 --test-success=3 --simulate=199

sudo ./responder --spb=64 --spp=64 --rate=4e6 --iterations=100 --delay-min=100e-6 --delay-max=5000e-6 --delay-step=50e-6 --duration=10e-6 --time-mul=1e6 --test-success=3 --simulate=199

sudo ./responder --spb=64 --spp=64 --rate=4e6 --iterations=100 --delay-min=100e-6 --delay-max=5000e-6 --delay-step=50e-6 --duration=10e-6 --time-mul=1e6 --test-success=3 --pdt 0


sudo ./responder --spb=64 --spp=64 --rate=4e6 --iterations=100 --delay-min=100e-6 --delay-max=300e-6 --delay-step=50e-6 --duration=10e-3 --time-mul=1e6 --test-success=3 --pdt .01 --rx-band 2e6 --tx-band 2e6


V1 CSG:
ADDED RX/TX frequency enteries
sudo ./responder --spb=128  --rate=4e6 --iterations=10000 --delay-min=100e-6 --delay-max=1500e-6 --delay-step=100e-6 --duration=500e-6 --time-mul=1e6 --pdt .001 --rx-band 2e6 --tx-band 2e6 --allow-late

sudo ./responder --spb=128  --rate=4e6 --iterations=100 --delay-min=100e-6 --delay-max=1500e-6 --delay-step=100e-6 --duration=500e-6 --time-mul=1e6 --pdt .001 --rx-band 2e6 --tx-band 2e6 --allow-late

sudo ./responder --spb=128  --rate=4e6 --iterations=5 --delay-min=300e-6 --delay-max=700e-6 --delay-step=200e-6 --duration=500e-6 --time-mul=1e6 --pdt .005 --rx-band 2e6 --tx-band 2e6 --allow-late --level 2



--help                                help message
  --args arg                            single uhd device address args
  
  --delay arg (=0.0050000000000000001)  number of seconds in the future to reply
  
  --rate arg (=1000000)                 rate of outgoing samples
  --level arg (=0.5)                    trigger level as fraction of high level
  --scale arg (=0.300000012)            output scaling
  --duration arg (=0.001)               duration of response (seconds)
  --dc-offset-delay arg (=0)            duration of DC offset calibration
                                        (seconds)
  --init-delay arg (=0.5)               initialisation delay (seconds)
  --timeout arg (=3)                    stream timeout (seconds)
  
  --spb arg (=1024)                     samples per buffer
  --spp arg (=0)                        samples per packet (0: use default)
  
  --calib arg (=0.5)                    level calibration duration (seconds)
  
  --invert                              input signal inversion
  --invert-output                       output signal inversion
  
  --no-delay                            disable timed delay
  --allow-late                          allow late bursts
  
  --stats-file arg                      test statistics output filename (empty:
                                        auto-generate)
  
  --delay-min arg (=0.0001)             minimum delay
  --delay-max arg (=0.0050000000000000001)
                                        maximum delay
  --delay-step arg (=9.9999999999999995e-07)
                                        delay step
  
  --pdt arg (=0.001)                    pulse detection threshold
  
  --iterations arg (=0)                 test iterations
  
  --test-duration                       treat test iterations as duration
  
  --test-success arg (=0)               end test after multiple successful
                                        delays (0: run through all delays)
  
  --skip-iterations arg (=50)           skip first iterations for each delay
  
  --simulate arg (=0)                   frequency of simulation event (Hz)
  --time-mul arg (=1)                   statistics output time multiplier
  --ignore-simulation-check             ignore if simulation rate exceeds
                                        maximum delay + response duration
  --stats-file-prefix arg               test statistics output filename prefix
  --stats-file-suffix arg               test statistics output filename suffix
  --flush arg (=16)                     number of zero samples to add to a
                                        burst to flush hardware
  --no-eob                              disable end-of-burst
  --adjust-simulation-rate              adjust simulation rate if it will be
                                        too fast for maximum delay duration
  --optimize-simulation-rate            make simulation rate as fast as
                                        possible for each delay
  --optimize-padding arg (=16)          time (as number of samples) to pad
                                        optimized simulation rate
  --no-stats-file                       do not output statistics file
  --batch-mode                          disable user prompts
