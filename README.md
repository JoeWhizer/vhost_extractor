# vhost_extractor 1.1 (boost version)
Extractor for Apache virtual hosts - now with colorful output

This program is helpful to strip all or a single virtual host by servername from a huge apache configuration file.

## Usage

**Usage:** vhost_extractor -i /path/to/apache.conf -o /path/to/output/dir

**Options:**

-i /path/to/apache.conf&emsp;&emsp;- Full path to the configuration that you want to parse

-o /path/to/output/dir&emsp;&emsp;&nbsp;&nbsp;- Path where extracted vhost will be stored (if not exists, it will be created)

-s servername&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;- Extract single configuration by ServerName

-v&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;&nbsp;- Verbose setting (prints out a result after parsing

-h&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;&nbsp;- Shows this help!
<br><br>

## Dependencies:

You need to install boost lib package.

**CentOS:**
yum install boost


**Arch:**
pacman -Syu boost-libs
<br><br>

## Compilation

Compile the program with the following command within src folder:
```
g++ main.cpp include/TBBTools.cpp include/TBBConsole.cpp -o ../bin/vhost_extractor -std=c++11 -lboost_system -lboost_filesystem
```
Or use the create.sh script to build and compile it with cmake. (don't forget to configure your cmake before, and adjust the project_path variable in the script)
<br><br>

For compilation you also need the devel-package of boost.

**CentOS7**: yum install boost-devel
**Arch**: pacman -Syu boost
