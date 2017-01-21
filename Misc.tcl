#################################################################################
# ngBot - Miscellaneous triggers                                                #
#################################################################################
# 
# Description:
#
# Basic plugin to echo the output of a script to the channel. Like listaffils.sh
#
# Installation:
# 1. Add the following to your eggdrop.conf:
#    source pzs-ng/plugins/Misc.tcl
#
# 2. Rehash or restart your eggdrop for the changes to take effect.
#
# Changelog:
# - 20150216
#
#################################################################################

namespace eval ::ngBot::plugin::Misc {
	variable ns [namespace current]
	variable np [namespace qualifiers [namespace parent]]

	variable misc
        variable scriptFile [info script]
        variable scriptName ${ns}::LogEvent

	## Config Settings ###############################
	##
	## Channel
	set misc(chan)       		"#pzs-ng"
	set misc(listaffilstrigger) 	"!affils"
        set misc(prefix)		"Affils: "
	##
	##################################################

	set misc(version) "20150216"

	proc init {args} {
		variable misc
                variable ns
                variable np
                variable scriptName
                variable scriptFile

		set theme_file [file normalize "[pwd]/[file rootname $scriptFile].zpt"]
                if {[file isfile $theme_file]} {
                        ${np}::loadtheme $theme_file true
                }

                if {([info exists misc(listaffilstrigger)]) && (![string equal $misc(listaffilstrigger) ""])} {
                        bind pub -|- $misc(listaffilstrigger) ${ns}::listAffils
                }

		putlog "\[ngBot\] Misc :: Loaded successfully (Version: $misc(version))."
	}

	proc deinit {args} {

                variable ns
                variable misc

                catch {unbind pub -|- $misc(listaffilstrigger) ${ns}::listAffils}
		namespace delete [namespace current]
	}

	proc listAffils {args} {
                variable misc

		if {[catch {exec /glftpd/bin/listaffils.sh} output] != 0} {
			putlog "\[ngBot\] Misc :: Error: Problem executing listaffils \"$output\""
			return
		}

		foreach line [split $output "\n"] {

			foreach chan [split $misc(chan)] {
				puthelp "PRIVMSG $chan :$misc(prefix) $line"
			}
		}
	}
}
