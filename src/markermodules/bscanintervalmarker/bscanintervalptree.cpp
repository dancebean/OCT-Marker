#include "bscanintervalptree.h"

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>


#include <boost/lexical_cast.hpp>

#include "bscanintervalmarker.h"


namespace bpt = boost::property_tree;

bool BScanIntervalPTree::parsePTree(const bpt::ptree& ptree, BScanIntervalMarker* markerManager)
{
	const char* bscansNodeStr = "IntervalMarker";
	boost::optional<const bpt::ptree&> bscansNode = ptree.get_child_optional(bscansNodeStr);

	if(!bscansNode)
		return false;


	for(const std::pair<const std::string, const bpt::ptree>& bscanPair : *bscansNode)
	{
		if(bscanPair.first != "BScan")
			continue;

		const bpt::ptree& bscanNode = bscanPair.second;
		int bscanId = bscanNode.get_child("ID").get_value<int>();


		boost::optional<const bpt::ptree&> quallityIntervallsNode = bscanNode.get_child_optional("QuallityIntervals");

		if(quallityIntervallsNode)
		{
			for(const std::pair<const std::string, const bpt::ptree>& intervallNodePair : *quallityIntervallsNode)
			{
				if(intervallNodePair.first != "Intervall")
					continue;

				const bpt::ptree& intervallNode = intervallNodePair.second;

				int         start          = intervallNode.get_child("Start").get_value<int>();
				int         end            = intervallNode.get_child("End"  ).get_value<int>();
				std::string intervallClass = intervallNode.get_child("Class").get_value<std::string>();

				try
				{
					IntervalMarker::Marker marker = IntervalMarker::getInstance().getMarkerFromString(intervallClass);
					markerManager->setMarker(start, end, marker, bscanId);
				}
				catch(std::out_of_range& r)
				{
					std::cerr << "unknown interval class " << intervallClass << " : " << r.what() << std::endl;
				}

			}
		}
	}
	return true;
}

void BScanIntervalPTree::fillPTree(bpt::ptree& markerTree, const BScanIntervalMarker* markerManager)
{
	bpt::ptree& qualityTree = markerTree.add("Quality", "");

	int numBscans = markerManager->getNumBScans();
	for(int bscan = 0; bscan < numBscans; ++bscan)
	{
		std::string nodeName = "BScan";
		bpt::ptree& bscanNode = qualityTree.add(nodeName, "");
		bscanNode.add("ID", boost::lexical_cast<std::string>(bscan));

		for(const BScanIntervalMarker::MarkerMap::interval_mapping_type pair : markerManager->getMarkers(bscan))
		{

			// std::cout << "paintEvent(QPaintEvent* event) " << pair.second << " - " << pair.first << std::endl;

			IntervalMarker::Marker marker = pair.second;
			if(marker.isDefined())
			{
				boost::icl::discrete_interval<int> itv  = pair.first;

				bpt::ptree& intervallNode = bscanNode.add("Intervall", "");

				intervallNode.add("Start", itv.lower());
				intervallNode.add("End"  , itv.upper());
				intervallNode.add("Class", marker.getName());

				// xmltree.add(name, value);
			}
		}
	}
}
