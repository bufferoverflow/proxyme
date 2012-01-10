/*
 * Copyright (c) 2011, Roger Meier <roger@bufferoverflow.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roger Meier <roger@bufferoverflow.ch> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <string>
#include <iostream>  
#include <fstream>

#include <ctemplate/template.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

#include "config.h"
#include "proxyme.tpl.varnames.h"

static std::string environment_mapper(const std::string &var_name) {
  if (var_name == "HOME")
    return var_name;
  return "";
}

int main(int argc, char** argv) {
  string proxy_user, proxy_pwd, proxy_host;
  string configfile;
  filesystem::path configdir;
  int proxy_port = 0;
  bool overwrite = false;
  bool disable = false;
  bool save = false;
  bool urlencode = false;
  filesystem::path home;
  property_tree::ptree pt;

  program_options::options_description desc("proxyme options");
  desc.add_options()("help,h", "produce help message")
      ("user", program_options::value<string>(&proxy_user)->default_value(proxy_user),
          "User      (e.g. DOMAIN\\user)")
      ("password", program_options::value<string>(&proxy_pwd)->default_value(proxy_pwd),
          "Password  (e.g. 1234)")
      ("host", program_options::value<string>(&proxy_host)->default_value(proxy_host),
          "Host      (e.g. proxy.example.com)")
      ("port", program_options::value<int>(&proxy_port)->default_value(proxy_port),
          "Port      (e.g. 85)")
      ("disable,d", program_options::value<bool>(&disable)->zero_tokens(),
          "Disable Proxy and overwrite all config files!")
      ("overwrite,o", program_options::value<bool>(&overwrite)->zero_tokens(),
          "Overwriting files, don't care if they exist!")
      ("save,s",  program_options::value<bool>(&save)->zero_tokens(),
          "Save current parameters within proxyme.ini file")
	  ("urlencode,u", program_options::value<bool>(&urlencode)->zero_tokens(),
	      "Store the password in URL encoded form")
      ("HOME", program_options::value<boost::filesystem::path>(&home)->default_value(home),
          "Environment Variable: HOME");

  program_options::variables_map options;
  program_options::store(program_options::parse_command_line(argc, argv, desc), options);
  program_options::store(program_options::parse_environment(desc, &environment_mapper), options);
  program_options::notify(options);

  if (options.count("help")) {
    cout << desc << endl;
    return 1;
  }

  configdir = home.string() + CONFIGDIR;
  configfile = configdir.directory_string() + CONFIGFILENAME;

  if (!filesystem::exists(configfile)) {
    cout << "No configuration available, initialization of proxyme...";
    if (filesystem::exists(DATADIR)) {
      filesystem::create_directory(configdir);
      filesystem::path dir = DATADIR;

      filesystem::directory_iterator end_it;

      for ( filesystem::directory_iterator it(dir); it != end_it; ++it) {
        filesystem::copy_file(it->path(), configdir.directory_string() + it->path().filename());
      }

      cout << " done." << endl;
      cout << "Customize here: " << configdir.directory_string() << endl;
      return 0;
      }
    else{
      cout << " error." << endl;
      cout << "Template directory " << DATADIR << " does not exist, did you made a proper install?" << endl;
      return 1;
    }
  }

  read_ini(configfile, pt);
  proxy_host = pt.get<string> ("proxyme.host", proxy_host);
  proxy_port = pt.get<int> ("proxyme.port", proxy_port);
  proxy_user = pt.get<string> ("proxyme.user", proxy_user);
  proxy_pwd = pt.get<string> ("proxyme.password", proxy_pwd);

  if(save) {
    cout << "save host, port and user to proxyme.ini" << endl;
    pt.put("proxyme.host", proxy_host);
    pt.put("proxyme.port", proxy_port);
    pt.put("proxyme.user", proxy_user);
    write_ini(configfile, pt);
  }

  if(!disable) {
    if (proxy_host.empty() || proxy_port == 0) {
      cout << "Proxy host and port is required." << endl;
      return 1;
    }

    if (!proxy_user.empty() && proxy_pwd.empty()) {
      cout << "Password is required if user is defined!" << endl;
      return 1;
    }
  }


  // fill the template dictionary
  ctemplate::TemplateDictionary dict("proxyme");
  dict.SetValue(kp_PROXY_HOST, proxy_host);
  dict.SetIntValue(kp_PROXY_PORT, proxy_port);

  if (!proxy_user.empty()) {
    dict.SetValue(kp_PROXY_USER, proxy_user);
	
	if ( urlencode ) {
		// Replace each character of proxy_pwd with its hex value prependen by a % sign
		// this is known as URL encoding (http://en.wikipedia.org/wiki/Percent-encoding)
		// This allows to have special characters in the password and gives a basic protection
		// against accidently revealing the password
		char hex[3];
		string enc_proxy_pwd = "";
		int i, length = proxy_pwd.length();
		for(i = 0; i < length; i++)
		{
	    	sprintf(hex, "%%%X",proxy_pwd[i]);
			enc_proxy_pwd.append(hex);
		}
		proxy_pwd = enc_proxy_pwd;
	}
    dict.SetValue(kp_PROXY_PWD, proxy_pwd);
    dict.ShowSection(kp_PROXY_AUTH);
  }

  if (disable) {
    dict.SetValue(kp_PROXYME, "proxy disabled by proxyme");
    cout << "Disable proxy" << endl;
    overwrite = true;
  } else {
    dict.SetValue(kp_PROXYME, "proxy enabled by proxyme");
    dict.ShowSection(kp_PROXY);
  }

  // read configuration
  read_ini(configfile, pt);

  for (property_tree::ptree::const_iterator it = pt.begin(); it != pt.end(); it++) {
    // ignore config section
    if(it->first == "proxyme") {
      it++;
    }

    string template_file = pt.get<string> (it->first + ".template");
    string target_file = pt.get<string> (it->first + ".target");

    if (!home.empty()) {
      regex pattern("[$][(](HOME)[)]", boost::regex_constants::perl);
      template_file = boost::regex_replace(template_file, pattern, home.string());
      target_file = boost::regex_replace(target_file, pattern, home.string());
    }

    if (pt.get_optional<bool> (it->first + ".escape_backslash")) {
      regex pattern("\\\\", boost::regex_constants::perl);
      string escaped_user = boost::regex_replace(proxy_user, pattern, "\\\\\\");
      dict.SetValue(kp_PROXY_USER, escaped_user);
    }

    if (filesystem::exists(target_file) && !overwrite) {
      cout << target_file << " exists, leave it untouched." << endl;
    } else {
      if (filesystem::exists(target_file) && overwrite) {
        cout << "overwrite " << target_file << endl;
      } else {
        cout << "write " << target_file << endl;
      }
      string output;
      ofstream genfile;
      ctemplate::ExpandTemplate(template_file, ctemplate::DO_NOT_STRIP, &dict, &output);
      genfile.open(target_file.c_str());
      genfile << output;
      genfile.close();
    }
  }

  return 0;
}

