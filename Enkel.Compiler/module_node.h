#pragma once
#include "base_node.h"
#include <memory>
#include <vector>

namespace enkel {
	namespace compiler {
		class module_node : public base_node
		{
		public:
			module_node(const std::string &name)
				: mName(name) {
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			std::string dump() override {
				return "";
			}

			void add_elem(std::unique_ptr<module_elem_node> elem) {
				mElems.push_back(move(elem));
			}

			const std::string &get_name() {
				return mName;
			}

			std::vector<std::unique_ptr<module_elem_node>>& get_elems() {
				return mElems;
			}

		private:
			std::vector<std::unique_ptr<module_elem_node>> mElems;
			std::string mName;
		};
	}
}
