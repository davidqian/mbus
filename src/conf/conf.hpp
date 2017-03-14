//
// con.hpp
// ~~~~~~~~~~~
//
//

#ifndef MBUS_CONF_HPP
#define MBUS_CONF_HPP
#include <string>
#include <vector>
namespace mbus {
   class conf {
	public:
    		static conf * getInstance() {
        		static conf * instance = nullptr;
        		if (!instance) {
            			instance = new conf();
        		}
        		return instance;
    		}
	private:
    		conf();
	public:
    		bool init(const string & iniPath , const string & exe_path, string & errMsg);
    		const string & getLogPath();
    		string getCallbackUrl();
    		string getTssConfigPath();
    		string getTssLibPath();
    		unsigned short getPort();
    		int getThreadCount();

	private:
    		string logPath_;
    		string tssConfigPath_;
    		string tssLibPath_;
    		unsigned short port_;
    		int threadCount_;
    		string iniPath_;
    		string callbackUrl_;
	};	
} // namespace mbus

#endif // MBUS_CONF_HPP
