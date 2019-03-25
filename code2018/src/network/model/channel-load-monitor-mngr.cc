/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 *
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
#include "channel-load-monitor-mngr.h"
#include "ns3/object.h"

NS_LOG_COMPONENT_DEFINE ("ChannelLoadMonitorMngr");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ChannelLoadMonitorMngr);

TypeId ChannelLoadMonitorMngr::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ChannelLoadMonitorMngr")
    .SetParent<Object> ()
    ;
  return tid;
}

ChannelLoadMonitorMngr::~ChannelLoadMonitorMngr ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
ChannelLoadMonitorMngr::SetNetDevice (Ptr<NetDevice> device)
{
  m_netDevice = device;
}

Ptr<NetDevice>
ChannelLoadMonitorMngr::GetNetDevice(void)
{
	return m_netDevice;
}

float
ChannelLoadMonitorMngr::GetChannelLoad (void)
{
  NS_LOG_INFO ("Obtaining the locally measured channel load");
  //std::cout<<"ChannelLoadMonitorMngr::getChannelLoad node "<<m_netDevice->GetNode()->GetId() <<" device "<<m_netDevice->GetIfIndex()<<" CL "<<m_channelLoad<<std::endl;
  return (m_channelLoad);
}

} //namespace ns3
