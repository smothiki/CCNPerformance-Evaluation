
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

using namespace ns3;


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
  
  consumerHelper.SetPrefix ("/NodeB1");
  consumerHelper.Install (consumer1);
  //ApplicationContainer consumerNode1=consumerHelper.Install (consumer1);	
  //consumerNode1.Start (Seconds (0.0));

  ndn::AppHelper consumerHelper2 ("ns3::ndn::ConsumerCbr");
  consumerHelper2.SetAttribute ("Frequency", StringValue ("1")); // 100 interests a second
  consumerHelper2.SetAttribute ("StartSeq", StringValue ("1"));

  consumerHelper2.SetPrefix ("/NodeB2");
  consumerHelper2.Install (consumer1);


  consumerHelper.SetAttribute ("Frequency", StringValue ("1")); // 100 interests a second
  consumerHelper.SetAttribute ("StartSeq", StringValue ("1"));

  consumerHelper.SetPrefix ("/NodeB1");
  consumerHelper.Install (consumer2);
  //ApplicationContainer consumerNode1=consumerHelper.Install (consumer1);  
  //consumerNode1.Start (Seconds (0.0));

  consumerHelper2.SetAttribute ("Frequency", StringValue ("1")); // 100 interests a second
  consumerHelper2.SetAttribute ("StartSeq", StringValue ("0"));

  consumerHelper2.SetPrefix ("/NodeB2");
  consumerHelper2.Install (consumer2);

  //consumerHelper.SetPrefix ("/NodeB2");
  //consumerHelper.Install (consumer2);

  //ApplicationContainer consumerNode2=consumerHelper.Install (consumer2);
  //consumerNode2.Start (Seconds (1.0));
  ndn::AppHelper producerHelper ("ns3::ndn::Producer");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));  

  ndnGlobalRoutingHelper.AddOrigins ("/NodeB1", producer1);
  producerHelper.SetPrefix ("/NodeB1");
  producerHelper.Install (producer1);


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
