/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Adel Mansouri <adel.mansouri@nxp.com>
 */

#ifndef NIST_CHANNEL_LOAD_MONITOR_MNGR_H
#define NIST_CHANNEL_LOAD_MONITOR_MNGR_H

#include "ns3/simulator.h"
#include "ns3/object.h"
#include "ns3/net-device.h"
#include "ns3/channel-load-monitor-mngr.h"


namespace ns3
{

class NISTChannelLoadMonitorMngr : public ChannelLoadMonitorMngr
{

public:
    static TypeId GetTypeId (void);
    virtual ~NISTChannelLoadMonitorMngr();
    void startMonitoring();
    void stopMonitoring();
    void Monitor();


protected:
  int64_t m_monitorWindow; // the monitoring window duration (1s by default)
  int64_t m_monitorInterval; // the monitoring interval (10ms by default, ~~slot time for 10MHz ITS-G5)
  int64_t m_lastUpdate;
  uint32_t m_probe;
  uint32_t m_total_busy;
  bool m_stopMonitoring;
  float m_previous_channelLoad;
  float m_weight;
  uint32_t m_tmp_probe;				//parameter useful to check the channel load trend inside a single window
  uint32_t m_tmp_busy; 				//parameter useful to check the channel load trend inside a single window
  int64_t m_tmp_lastUpdate; 			//parameter useful to check the channel load trend inside a single window
  uint32_t m_overlapped_probe;		// for overlapped window
  uint32_t m_overlapped_busy; 		// for overlapped window
  uint32_t m_prev_overlapped_probe ;	// for overlapped window
  uint32_t m_prev_overlapped_busy ;	// for overlapped window
  uint32_t m_taps_number;				// EMA filter lenght
  std::vector<float > m_weights;		// EMA weights
  std::vector<float > m_taps;			// EMA samples
  std::vector<float > m_past_samples;	// ETSI filter

};


}


#endif


