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


#ifndef INCLUDED_FPGNU_FPDATA_SOURCE_H
#define INCLUDED_FPGNU_FPDATA_SOURCE_H

#include <fpgnu/api.h>
#include <gr_block.h>

namespace gr {
  namespace fpgnu {

    /*!
     * \brief <+description of block+>
     * \ingroup block
     *
     */
    class FPGNU_API fpdata_source : virtual public gr_block
    {
    public:
       typedef boost::shared_ptr<fpdata_source> sptr;

       /*!
        * \brief Return a shared_ptr to a new instance of fpgnu::fpdata_source.
        *
        * To avoid accidental use of raw pointers, fpgnu::fpdata_source's
        * constructor is in a private implementation
        * class. fpgnu::fpdata_source::make is the public interface for
        * creating new instances.
        */
       static sptr make();
    };

  } // namespace fpgnu
} // namespace gr

#endif /* INCLUDED_FPGNU_FPDATA_SOURCE_H */
