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

/*
 * \file   OLSR_rtable.cc
 * \brief  Implementation of network coding table.
 */

#include "OLSR.h"
#include "OLSR_nc_table.h"
#include "OLSR_repositories.h"


OLSR_nc_table::OLSR_nc_table()
{
}

OLSR_nc_table::~OLSR_nc_table()
{
    for (nctable_t::iterator it = nct_.begin(); it != nct_.end(); it++)
        delete (*it).second;
}

/*
 * \brief Clears the table and frees the memory assigned to each one of its entries.
 */
void OLSR_nc_table::clear()
{
    // Iterates over the nc table deleting each OLSR_nc_entry*.
    for (nctable_t::iterator it = nct_.begin(); it != nct_.end(); it++)
        delete (*it).second;

    // Cleans routing table.
    nct_.clear();
}

/*
 * \brief Deletes the entry whose generation given.
 */
void OLSR_nc_table::rm_entry(int gen)
{
    // Remove the pair whose key is gen
    nct_.erase(gen);
}


/*
 * \brief Looks up an entry for the specified generation.
 * \param generation.
 * \return the routing table entry for that destination address, or NULL
 * if such an entry does not exist
 */
OLSR_nc_entry* OLSR_nc_table::lookup(int gen)
{
    // Get the iterator at "gen" position
    nctable_t::iterator it = nct_.find(gen);
    // If there is no route to "dest", return NULL
    if (it == nct_.end())
        return NULL;

    // Returns the nct entry (second element of the pair)
    return (*it).second;
}


/*
 * \brief Add a new entry into the nc table
 */
void OLSR_nc_table::add_entry(int gen, OLSR_nc_entry* new_entry){
	// Inserts the new entry
	nctable_t::iterator it = nct_.find(gen);
	if (it != nct_.end()){ //if it already exist
		delete new_entry;	// delete since that object will never be used
		return;				// do nothing
	}
	nct_[gen] = new_entry;	//otherwise save the new entry
}


/*
 *  \brief Returns the number of entries in the routing table.
 *  \return the number of entries in the routing table.
 */
int OLSR_nc_table::size()
{
    return nct_.size();
}


/*
 * \brief Prints out the content of the routing table
 *
 * Content is represented as a table in which each line is preceeded by a 'P'.
 * First line contains the name of every column (dest, next, iface, dist)
 * and the following ones are the values of each entry.
 */
std::string OLSR_nc_table::detailedInfo()
{
    std::stringstream out;

    for (nctable_t::iterator it = nct_.begin(); it != nct_.end(); it++)
    {
    	out << "Generation: "<< ((*it).first);
    	out << " decoded packets: "<< ((*it).second)->decoded_pkts();
    	out << "  total packets: "<< ((*it).second)->total_pkts();

        //OLSR_rt_entry* entry = dynamic_cast<OLSR_rt_entry *> ((*it).second);
        out <<"\n";
    }
    return out.str();
}
