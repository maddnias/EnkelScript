#pragma once

#include "base_ast_visitor.h"

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
		};
	}
}
