#pragma once
#include "exec_visitor.h"
#include "rt_var.h"

namespace enkel {
	namespace runtime {
		class call_visitor :
			public exec_visitor
		{
		public:
			call_visitor(enkel_runtime &rt, compiler::func_decl_node &targetFunc);
			~call_visitor();

			void visit(compiler::call_expr_node &node) override;
			bool has_retval() const;
			variant_datatype get_retval() const;

			void visit(compiler::block_node &node) override;
		private:
			variant_datatype mRetVal;
			compiler::func_decl_node &mTargetFunc;
			bool mRecursive;
		};
	}
}
