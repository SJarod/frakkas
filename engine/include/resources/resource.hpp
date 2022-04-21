#pragma once

#include <string>

namespace Resources
{
	class Resource
	{
	public:
		virtual ~Resource() = default;

		// Resource name or path
		std::string name;

		/**
		* Load the resource from each resource specific info (name, ...).
		* Override this function in the child class.
		*/
		virtual void LoadFromInfo() = 0;
	};
}