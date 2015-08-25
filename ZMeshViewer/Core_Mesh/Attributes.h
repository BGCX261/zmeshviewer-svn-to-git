#ifndef _XMESHLIB_CORE_ATTRIBUTES_H_
#define _XMESHLIB_CORE_ATTRIBUTES_H_

#include <hash_map>

//#pragma warning (disable : 4786)

namespace XMeshLib
{

	template <class T1, class T2> class Attributes
	{
	public:
		Attributes()
		{;}

		~Attributes()
		{;}

		bool valid()
		{
			return (data.size())?true:false;
		}

		void clear()
		{
			data.clear();
		}

		void add(T1 t1, T2 t2)
		{
			data.insert(std::pair<T1, T2>(t1, t2)); //.push_back(t1,t2);
		}

		T2 & operator [] (T1 t1)
		{
			stdext::hash_map<T1, T2>::iterator cpos = data.find(t1);
			if (cpos==data.end())
				assert(0);
			return cpos->second;
			//assert(ind >= 0 && ind < (signed int)data.size());
			//return data[ind];
		}

	private:
		//enum AttributePrimitiveType pType;		
		stdext::hash_map<T1, T2> data;
		//std::vector<T> data;
	};




}

#endif