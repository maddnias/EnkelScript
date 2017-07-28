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
				VAR_TYPE_DOUBLE,
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

			bool operator<(variant_datatype &other) const;
			bool operator<=(variant_datatype &other) const;
			bool operator>(variant_datatype &other) const;
			bool operator>=(variant_datatype &other) const;
			bool operator>=(const int &rhs) const;
			bool operator<(const int &rhs) const;

			bool is_num() const;
			variant_datatype& operator+(variant_datatype &other);
			variant_datatype& operator+=(variant_datatype &other);

			variant_datatype& operator*(variant_datatype &other);
			variant_datatype& operator*=(variant_datatype &other);

			bool operator==(variant_datatype &other) const;
			bool operator!=(variant_datatype &other) const;

			friend std::wostream& operator<<(std::wostream& os, const variant_datatype &var);

			var_type get_type() const;
			bool is_empty() const;

			std::wstring val_as_string() const;
			int val_as_i32() const;
			int64_t val_as_i64() const;
			double val_as_double() const;

			void change_to_i32();
			void change_to_i64();
			void change_to_str();
			void change_to_double();

			static variant_datatype empty();

		private:
			static var_type get_comp_type(var_type ty1, var_type ty2);

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
			bool mEmpty;
		};
	}
}

