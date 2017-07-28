#pragma once
#include <ostream>
#include <memory>
#include "include/variant_datatype.h"
#include "ast/expr_node.h"

namespace enkel {
	namespace runtime {
		/**
		 * \brief Class to hold any parameter used in Enkel STL functions
		 */
		class enkel_stl_param_wrapper {
		public:
			explicit enkel_stl_param_wrapper(std::wostream *outStream)
				: mOutStream(outStream),
				  mInStream(nullptr) {
			}


			explicit enkel_stl_param_wrapper(std::istream *inStream)
				: mOutStream(nullptr),
				  mInStream(inStream) {
			}


			explicit enkel_stl_param_wrapper(std::unique_ptr<variant_datatype> dt)
				: mOutStream(nullptr),
				  mInStream(nullptr),
				  mVariant(move(dt)) {
			}

			std::wostream& get_out_stream() const {
				return *mOutStream;
			}

			std::istream& get_in_stream() const {
				return *mInStream;
			}

			variant_datatype& get_variant() const {
				return *mVariant;
			}

			std::unique_ptr<variant_datatype> take_variant() {
				return move(mVariant);
			}

		private:
			std::wostream *mOutStream;
			std::istream *mInStream;
			std::unique_ptr<variant_datatype> mVariant;
		};
	}
}
