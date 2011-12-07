/***************************************************************************
 *   Copyright (C) 2011 by Matteo Bruni                                    *
 *   matteo.bruni@gmail.com                                                *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __OLSR_nc_table_h__
#define __OLSR_nc_table_h__

#include "OLSR_repositories.h"
#include <omnetpp.h>
#include <map>


/*
 * \brief Defines nctable_t as a map of OLSR_nc_entry, whose key is the packets generation.
 *
 * The network coding table is thus defined as pairs: [generation, entry]. Each element
 * of the pair can be accesed via "first" and "second" members.
 */
typedef std::map<int, OLSR_nc_entry*> nctable_t;

/*
 * Network Coding Table
 */
class OLSR_nc_table : public cObject
{
	nctable_t    nct_;    ///< Data structure for the network coding table.

  public:

	OLSR_nc_table();
    ~OLSR_nc_table();

    void        clear();
    void        rm_entry(int gen);
    int    size();

    void add_entry(int gen, OLSR_nc_entry* new_entry);

    OLSR_nc_entry*  lookup(int gen);

    virtual std::string detailedInfo();
};

#endif
