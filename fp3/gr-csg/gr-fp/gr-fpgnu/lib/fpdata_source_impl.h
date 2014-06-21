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

#ifndef INCLUDED_FPGNU_FPDATA_SOURCE_IMPL_H
#define INCLUDED_FPGNU_FPDATA_SOURCE_IMPL_H

#include <fpgnu/fpdata_source.h>

namespace gr {
  namespace fpgnu {

    class fpdata_source_impl : public fpdata_source
    {
    private:
      // Nothing to declare in this block.

    public:
      fpdata_source_impl();
      ~fpdata_source_impl();

      // Where all the action really happens
      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace fpgnu
} // namespace gr

#endif /* INCLUDED_FPGNU_FPDATA_SOURCE_IMPL_H */

