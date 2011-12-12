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

    nct_.clear();
}

/*
 * \brief Deletes the entry whose generation given.
 */
void OLSR_nc_table::rm_entry(unsigned int gen)
{
	//printf("\n1n#############MORTEEE\n");
    // Remove the pair whose key is gen
    nctable_t::iterator it = nct_.find(gen);
    if (it != nct_.end())
        delete (*it).second;

    nct_.erase(it);//gen);
    //printf("\n###########MORTEEE2\n");

}


/*
 * \brief Looks up an entry for the specified generation.
 * \param generation.
 * \return the routing table entry for that destination address, or NULL
 * if such an entry does not exist
 */
OLSR_nc_entry* OLSR_nc_table::lookup(unsigned int gen)
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
void OLSR_nc_table::add_entry(unsigned int gen, OLSR_nc_entry* new_entry){
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
uint32_t OLSR_nc_table::size()
{
    return nct_.size();
}


/*
 * \brief Prints out the content of the nc table
 *
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


///////////////////////////////////////////


OLSR_nc_glob_table::OLSR_nc_glob_table()
{
}

OLSR_nc_glob_table::~OLSR_nc_glob_table()
{
}

/*
 * \brief Clears the table and frees the memory assigned to each one of its entries.
 */
void OLSR_nc_glob_table::clear()
{
	nc_g_t_.clear();
}
/*
 * \brief Deletes the entry whose address given.
 */
void OLSR_nc_glob_table::rm_entry(const nsaddr_t &dest, unsigned int generation)
{
	// Get the iterator at "dest" position
	nc_glob_table_t::iterator it = nc_g_t_.find(dest);
	// If there is no route to "dest", return NULL
	if (it == nc_g_t_.end())
		return;

	//(*it).second.rm_entry(generation);

	nctable_t& inner_map = (*it).second;

    nctable_t::iterator it2 = inner_map.find(generation);
    if (it2 != inner_map.end())
        delete (*it2).second;

    inner_map.erase(it2);//gen);

	if(inner_map.size() == 0){
		nc_g_t_.erase(dest);
	}

}

/*
 * \brief Looks up an entry for the specified source_addr and generation.
 * \param generation.
 * \return the routing table entry for that destination address, or NULL
 * if such an entry does not exist
 */
OLSR_nc_entry* OLSR_nc_glob_table::lookup(const nsaddr_t &dest, unsigned int generation)
{
    // Get the iterator at "dest" position
	nc_glob_table_t::iterator it = nc_g_t_.find(dest);
    // If there is no route to "dest", return NULL
    if (it == nc_g_t_.end())
        return NULL;

	nctable_t& inner_map = (*it).second;

    nctable_t::iterator it2 = inner_map.find(generation);
    // If there is no route to "dest", return NULL
    if (it2 == inner_map.end())
        return NULL;

    // Returns the nct entry (second element of the pair)
    return (*it2).second;



//
//    // get the generation entry
//	OLSR_nc_entry* entry = (*it).second.lookup(generation);
//
//    // Returns the nct entry
//    return entry;
}

/*
 * \brief Add a new entry into the table
 */
void OLSR_nc_glob_table::add_entry(const nsaddr_t &dest, unsigned int generation, OLSR_nc_entry* new_entry){
	// Inserts the new entry
	nc_glob_table_t::iterator it = nc_g_t_.find(dest);
	if (it != nc_g_t_.end()){ 	//if it already exist

		nctable_t& inner_map = (*it).second;
		nctable_t::iterator it2 = inner_map.find(generation);
		if (it2 != inner_map.end()){ //if it already exist
			delete new_entry;	// delete since that object will never be used
			return;				// do nothing
		}
		inner_map[generation] = new_entry;
		return;					// do nothing

	}

	nc_g_t_[dest][generation] = new_entry;


	//(*it).second.add_entry(generation, new_entry);
	//[generation] = new_entry;	//otherwise save the new entry
}

/*
 *  \brief Returns the number of entries in the routing table.
 *  \return the number of entries in the routing table.
 */
uint32_t OLSR_nc_glob_table::size()
{
    return nc_g_t_.size();
}


/*
 * \brief Prints out the content of the nc table
 *
 */
std::string OLSR_nc_glob_table::detailedInfo()
{
    std::stringstream out;

    for (nc_glob_table_t::iterator it = nc_g_t_.begin(); it != nc_g_t_.end(); it++)
    {
    	nctable_t& inner_map = (*it).second;

    	out << "source: "<< OLSR::node_id((*it).first)<< "\n";

		for (nctable_t::iterator it2 = inner_map.begin(); it2 != inner_map.end(); it2++)
		{
			out << "\tGeneration: "<< ((*it2).first);
			out << " decoded packets: "<< ((*it2).second)->decoded_pkts();
			out << " total packets: "<< ((*it2).second)->total_pkts();

			//OLSR_rt_entry* entry = dynamic_cast<OLSR_rt_entry *> ((*it).second);
			out <<"\n";
		}

    	out <<"\n";
    }
    return out.str();
}
