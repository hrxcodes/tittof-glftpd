
AUTHOR: glucose


1) mkv_check.sh relies on mkvinfo in order to validate mkv files. mkvinfo must therefore
   be included in your glftpd/bin directory. The easiest way to do this is by installing
   the mkvtoolnix package on your Linux distribution of choice using your package manager
   
   i.e. apt-get install mkvtoolnix

   Please refer to http://www.bunkus.org/videotools/mkvtoolnix/downloads.html for further 
   installation instructions.
   
   Include mkvinfo in the needed_bins variable in your glftpd/libcopy.sh and run libcopy.sh.
   You should then find mkvinfo in glftpd/bin and all libraries it depends on in their 
   respective directories.

2) copy mkv_check.sh to glftpd/bin

3) customize the settings in mkv_check.sh

4) include mkv_check.sh in the glftpd.conf by replacing your post_check with

   post_check /bin/mkv_check.sh *
   
5) copy mkv_check.tcl and mkv_check.zpt to pzs-ng/plugins

6) load mkv_check.tcl by adding
   
   source pzs-ng/plugins/mkv_check.tcl
   
   to your eggdrop configuration file AFTER the source for ngBot.tcl
   
7) customise the announce for passed/failed mkv uploads in mkv_check.zpt

   NOTE: you can disable announces for mkv's passing the checks by including

   set disable(MKV_PASS) 1

   in your ngBot.conf
   
   
