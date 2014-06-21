/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include "cc_handle_impl.h"

namespace gr {
  namespace cc_proc {

    cc_handle::sptr
    cc_handle::make(cd_idto)
    {
      return gnuradio::get_initial_sptr (new cc_handle_impl(cd_idto));
    }

    /*
     * The private constructor
     */
    cc_handle_impl::cc_handle_impl(cd_idto)
      : gr_block("cc_handle",
		      gr_make_io_signature(<+MIN_IN+>, <+MAX_IN+>, sizeof (<+float+>)),
		      gr_make_io_signature(<+MIN_IN+>, <+MAX_IN+>, sizeof (<+float+>)))
    {}

    /*
     * Our virtual destructor.
     */
    cc_handle_impl::~cc_handle_impl()
    {
    }

    int
    cc_handle_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];

        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }


  } /* namespace cc_proc */
} /* namespace gr */

