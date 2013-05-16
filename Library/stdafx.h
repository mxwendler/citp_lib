

#ifndef STDAFX_H_
#define STDAFX_H_


#ifdef WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
#endif

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/socket.h>

#include <map>
#include <list>
#include <vector>
#include <sstream>

//
// for_each: convenience macro for iterating vectors
template < class T > class foreach_vector : public std::vector<T>
{
	public :
		bool first(){ it = std::vector<T>::begin(); return it != std::vector<T>::end(); }
		bool next(){ ++it; return it != std::vector<T>::end(); }
		bool previous(){ 
							if(it==std::vector<T>::begin()) return false; 
							else { --it; return true; } 
						}
		T current(){ return *it; }
		void erase_current(){ std::vector<T>::erase(it); }

		foreach_vector()
			:
			std::vector<T>(),
			it(std::vector<T>::begin())
			{
			}
		~foreach_vector()
			{
			}

		foreach_vector(const std::vector<T>& rhs)
			:
			std::vector<T>(),
			it(std::vector<T>::begin())
			{
				copy_over(rhs);
			}

		foreach_vector(const foreach_vector<T>& rhs)
			:
			std::vector<T>(),
			it(std::vector<T>::begin())
			{
				copy_over(rhs);
			}


		foreach_vector<T>& operator = (const foreach_vector<T>& rhs)
			{
				it = std::vector<T>::begin();
				copy_over(rhs);
				return *this;
			}

		//
		// hwcode
		bool index_ok(int i)
			{
				return i>=0 && i<static_cast<int>(std::vector<T>::size());
			}

protected:
		typename std::vector<T>::iterator it;

private:

		void copy_over(const std::vector<T>& other)
		{
			std::vector<T>::assign(other.begin(),other.end());
		}

		void copy_over(const foreach_vector<T>& other)
		{
			std::vector<T>::assign(other.begin(),other.end());
		}



};

#define JOIN2(a,b) a##b
#define JOIN(a,b) JOIN2(a,b)
#define foreach(a) for(bool JOIN(flag,__LINE__)=a.first(); JOIN(flag,__LINE__);JOIN(flag,__LINE__)=a.next())




#endif //STDAFX_H_
