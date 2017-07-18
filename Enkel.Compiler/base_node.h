#pragma once
#include "compiler_type.h"
#include "base_ast_visitor.h"
#include <memory>

namespace enkel {
	namespace compiler {
		/**
		 * \brief Base class for all expression nodes in the AST
		 */
		class base_node {
		public:
			virtual ~base_node() {
				
			}
			virtual void accept(base_ast_visitor &visitor) = 0;
			virtual std::string dump() = 0;
		protected:
			typedef compiler_type::type type;
		};
	}
}
