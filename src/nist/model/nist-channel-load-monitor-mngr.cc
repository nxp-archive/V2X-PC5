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

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "nist-channel-load-monitor-mngr.h"
#include "ns3/nist-lte-ue-phy.h"

NS_LOG_COMPONENT_DEFINE ("NISTChannelLoadMonitorMngr");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (NISTChannelLoadMonitorMngr);
TypeId NISTChannelLoadMonitorMngr::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NISTChannelLoadMonitorMngr")
    .SetParent<ChannelLoadMonitorMngr> ()
    .AddConstructor<NISTChannelLoadMonitorMngr> ()
    /*.AddAttribute ("MonitorWindow", "The length of the monitoring window",
                       UintegerValue (200),
                       MakeUintegerAccessor (&NISTChannelLoadMonitorMngr::m_monitorWindow),
                       MakeUintegerChecker<int64_t> (1))
    .AddAttribute ("MonitorInterval", "The length of the monitoring interval",
                       UintegerValue (10),
                       MakeUintegerAccessor (&NISTChannelLoadMonitorMngr::m_monitorInterval),
                       MakeUintegerChecker<int64_t> (1))*/
    ;
  return tid;
}

NISTChannelLoadMonitorMngr::~NISTChannelLoadMonitorMngr ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
NISTChannelLoadMonitorMngr::Monitor() {


}

void
NISTChannelLoadMonitorMngr::startMonitoring() {
	m_lastUpdate = Simulator::Now().GetMilliSeconds();
		//m_lastUpdate = 0;
		m_probe=0;
		m_total_busy=0;
		m_previous_channelLoad=0;
		m_weight=0.5;
		m_tmp_probe=0;
		m_tmp_busy=0;
	    m_overlapped_probe=0;
		m_overlapped_busy=0;
		m_prev_overlapped_probe = 0;
		m_prev_overlapped_busy = 0;
		m_taps_number = 7;

		for (unsigned i=0; i<7; ++i){
					m_past_samples.push_back(0);
				}

		Time nextTime(MilliSeconds (m_monitorInterval)); // Time till next probe
		NS_LOG_LOGIC ("nextProbe = " << nextTime);
		Simulator::Schedule(nextTime, &NISTChannelLoadMonitorMngr::Monitor, this);
}

void
NISTChannelLoadMonitorMngr::stopMonitoring() {
	m_stopMonitoring = true;
}


} //namespace ns3

