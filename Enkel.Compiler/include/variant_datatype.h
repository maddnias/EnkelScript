#pragma once
#include <string>


namespace enkel {
	namespace runtime {
		class variant_datatype
		{
		public:
			enum var_type {
				VAR_TYPE_STRING,
				VAR_TYPE_I32,
				VAR_TYPE_I64,
				VAR_TYPE_UI64,
				VAR_TYPE_FLOAT,
				VAR_TYPE_PTR
			};

			variant_datatype();
			variant_datatype(std::wstring val);
			variant_datatype(int val);
			variant_datatype(int64_t val);
			variant_datatype(uint64_t val);
			variant_datatype(double val);
			~variant_datatype();

			explicit operator bool() const;
			variant_datatype& operator=(const variant_datatype &variant2);
			variant_datatype& operator=(int i32dat);
			variant_datatype& operator=(int64_t i64dat);
			variant_datatype& operator=(void *ptrDat);
			variant_datatype& operator=(double floatDat);
			variant_datatype& operator=(std::wstring strDat);
			variant_datatype& operator+(variant_datatype &other);
			variant_datatype& operator*(variant_datatype &other);
			bool operator==(variant_datatype &other) const;
			bool operator!=(variant_datatype &other) const;

			friend std::wostream& operator<<(std::wostream& os, const variant_datatype &var);

			var_type get_type() const;
			std::wstring to_string() const;

		private:
			double val_as_double() const;
			void reset_var(var_type type = VAR_TYPE_I32);
			static bool ensure_addition_possible(var_type lType, var_type rType);
		
			bool ensure_type(var_type type) const;

			var_type mType;
			std::wstring mStrVal;
			int mI32Val;
			int64_t mI64Val;
			uint64_t mUI64Val;
			double mFVal;
			void *mPtrVal;
		};
	}
}

