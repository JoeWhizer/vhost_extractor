# vhost_extractor 1.0
Extractor for Apache virtual hosts

This program is helpful to strip all or a single virtual host by servername from a huge apache configuration file.

## Usage

**Usage:** vhost_extractor -i /path/to/apache.conf -o /path/to/output/dir

**Options:**

-i /path/to/apache.conf&emsp;&emsp;- Full path to the configuration that you want to parse

-o /path/to/output/dir&emsp;&emsp;&nbsp;&nbsp;- Path where extracted vhost will be stored (if not exists, it will be created)

-s servername&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;- Extract single configuration by ServerName

-v&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;&nbsp;- Verbose setting (prints out a result after parsing

-h&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;&nbsp;- Shows this help!
