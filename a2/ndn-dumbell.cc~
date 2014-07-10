/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */
// ndn-congestion-topo-plugin.cc
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

using namespace ns3;

/**
 * This scenario simulates a grid topology (using topology reader module)
 *
 *   /------\	                                                 /------\
 *   | Src1 |<--+                                            +-->| Dst1 |
 *   \------/    \                                          /    \------/
 *            	 \                                        /     
 *                 +-->/------\   "bottleneck"  /------\<-+      
 *                     | Rtr1 |<===============>| Rtr2 |         
 *                 +-->\------/                 \------/<-+      
 *                /                                        \
 *   /------\    /                                          \    /------\
 *   | Src2 |<--+                                            +-->| Dst2 |
 *   \------/                                                    \------/
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-congestion-topo-plugin
 */

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  AnnotatedTopologyReader topologyReader ("", 25);
  topologyReader.SetFileName ("scratch/topo-6-node.txt");
  topologyReader.Read ();

  // Install NDN stack on all nodes
  /*ndn::StackHelper ndnHelper;
  ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute");
  ndnHelper.SetContentStore ("ns3::ndn::cs::Lru",
                              "MaxSize", "10000");
  ndnHelper.InstallAll ();

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll ();*/

  // Getting containers for the consumer/producer
  Ptr<Node> consumer1 = Names::Find<Node> ("NodeA1");
  Ptr<Node> consumer2 = Names::Find<Node> ("NodeA2");

  Ptr<Node> producer1 = Names::Find<Node> ("NodeB1");
  Ptr<Node> producer2 = Names::Find<Node> ("NodeB2");

  Ptr<Node> router1 = Names::Find<Node> ("NodeA");
  Ptr<Node> router2 = Names::Find<Node> ("NodeB");

  ndn::StackHelper ndnHelper;
  ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute");
  ndnHelper.SetContentStore ("ns3::ndn::cs::Nocache");
  ndnHelper.Install (consumer1);
  ndnHelper.Install (consumer2);
  ndnHelper.Install (producer1);
  ndnHelper.Install (producer2);
  ndnHelper.SetContentStore ("ns3::ndn::cs::Lru", "MaxSize", "3");
  ndnHelper.Install (router1);
  ndnHelper.Install (router2);

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll ();

  ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
  consumerHelper.SetAttribute ("Frequency", StringValue ("1")); // 100 interests a second
  consumerHelper.SetAttribute ("StartSeq", StringValue ("0"));
  // on the first consumer node install a Consumer application
  // that will express interests in /dst1 namespace
  consumerHelper.SetPrefix ("/NodeB1");
  consumerHelper.Install (consumer1);
  //ApplicationContainer consumerNode1=consumerHelper.Install (consumer1);	
  //consumerNode1.Start (Seconds (0.0));

  ndn::AppHelper consumerHelper2 ("ns3::ndn::ConsumerCbr");
  consumerHelper2.SetAttribute ("Frequency", StringValue ("1")); // 100 interests a second
  consumerHelper2.SetAttribute ("StartSeq", StringValue ("1"));
  // on the first consumer node install a Consumer application
  // that will express interests in /dst1 namespace
  consumerHelper2.SetPrefix ("/NodeB2");
  consumerHelper2.Install (consumer1);


  consumerHelper.SetAttribute ("Frequency", StringValue ("1")); // 100 interests a second
  consumerHelper.SetAttribute ("StartSeq", StringValue ("1"));
  // on the first consumer node install a Consumer application
  // that will express interests in /dst1 namespace
  consumerHelper.SetPrefix ("/NodeB1");
  consumerHelper.Install (consumer2);
  //ApplicationContainer consumerNode1=consumerHelper.Install (consumer1);  
  //consumerNode1.Start (Seconds (0.0));

  consumerHelper2.SetAttribute ("Frequency", StringValue ("1")); // 100 interests a second
  consumerHelper2.SetAttribute ("StartSeq", StringValue ("0"));
  // on the first consumer node install a Consumer application
  // that will express interests in /dst1 namespace
  consumerHelper2.SetPrefix ("/NodeB2");
  consumerHelper2.Install (consumer2);
  // on the second consumer node install a Consumer application
  // that will express interests in /dst2 namespace
  //consumerHelper.SetPrefix ("/NodeB2");
  //consumerHelper.Install (consumer2);

  //ApplicationContainer consumerNode2=consumerHelper.Install (consumer2);
  //consumerNode2.Start (Seconds (1.0));
  ndn::AppHelper producerHelper ("ns3::ndn::Producer");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));  

  // Register /dst1 prefix with global routing controller and
  // install producer that will satisfy Interests in /dst1 namespace
  ndnGlobalRoutingHelper.AddOrigins ("/NodeB1", producer1);
  producerHelper.SetPrefix ("/NodeB1");
  producerHelper.Install (producer1);

  // Register /dst2 prefix with global routing controller and
  // install producer that will satisfy Interests in /dst2 namespace
  ndnGlobalRoutingHelper.AddOrigins ("/NodeB2", producer2);
  producerHelper.SetPrefix ("/NodeB2");
  producerHelper.Install (producer2);

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes ();

  Simulator::Stop (Seconds (5.0));
  
  //ndn::L3AggregateTracer::InstallAll ("aggregate-trace.txt", Seconds (0.5));
  ndn::AppDelayTracer::InstallAll ("app-delays-trace.txt");
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
