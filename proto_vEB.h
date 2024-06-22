#pragma once
#include<cmath>
#include<array>
#ifndef fantasil_proto_veb_node_h
#define fantasil_proto_veb_node_h

namespace fantasil {



	template<typename T,size_t U>
	struct proto_vEB {
		using value_type = T;

		constexpr size_t u = U;
		constexpr size_t qu = static_cast<size_t>(std::sqrt(u));

		proto_vEB<value_type, qu>* sumary;
		std::array<proto_vEB<value_type, qu>*, qu> cluster;
	};


}

#endif
