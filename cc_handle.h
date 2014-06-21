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


#ifndef INCLUDED_CC_PROC_CC_HANDLE_H
#define INCLUDED_CC_PROC_CC_HANDLE_H

#include <cc_proc/api.h>
#include <gr_block.h>

namespace gr {
  namespace cc_proc {

    /*!
     * \brief <+description of block+>
     * \ingroup block
     *
     */
    class CC_PROC_API cc_handle : virtual public gr_block
    {
    public:
       typedef boost::shared_ptr<cc_handle> sptr;

       /*!
        * \brief Return a shared_ptr to a new instance of cc_proc::cc_handle.
        *
        * To avoid accidental use of raw pointers, cc_proc::cc_handle's
        * constructor is in a private implementation
        * class. cc_proc::cc_handle::make is the public interface for
        * creating new instances.
        */
       static sptr make(cd_idto);
    };

  } // namespace cc_proc
} // namespace gr

#endif /* INCLUDED_CC_PROC_CC_HANDLE_H */

