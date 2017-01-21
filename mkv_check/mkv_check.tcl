
namespace eval ::ngBot::plugin::MkvCheck {
    variable ns [namespace current]
    variable np [namespace qualifiers [namespace parent]]

    variable listenEvents { MKV_FAIL MKV_PASS }

    variable scriptFile [info script]
    variable scriptName ${ns}::LogEvent

    ### START - ngBot methods
    #
    
    proc init {args} {
        variable np
        variable ns
        variable listenEvents

        variable ${np}::msgtypes
        variable ${np}::precommand
        variable ${np}::variables

        variable scriptName
        variable scriptFile

        set theme_file [file normalize "[pwd]/[file rootname $scriptFile].zpt"]
        if {[file isfile $theme_file]} {
            ${np}::loadtheme $theme_file true
        }

        # register the event handlers
        foreach event $listenEvents {
            lappend msgtypes(SECTION) $event
            set variables($event) "%pf %mkv_name %mkv_expected %mkv_received %mkv_formatted_expected %mkv_formatted_received"            
            
            lappend precommand($event) $scriptName
        }

        putlog "mkv_check.tcl version 1.0 loaded"

        return
    }

    proc deinit {args} {
        variable np
        variable ${np}::precommand
        variable ${np}::msgtypes
        variable scriptName
        variable listenEvents
        
        # remove event handler
        foreach event $listenEvents {
            set idx [lsearch $msgtypes(SECTION) $event]
            set msgtypes(SECTION) [lreplace $msgtypes(SECTION) $idx $idx]        
        
            if {[info exists precommand($event)] && [set pos [lsearch -exact $precommand($event) $scriptName]] !=  -1} {
                set precommand($event) [lreplace $precommand($event) $pos $pos]
            }
        }

        namespace delete [namespace current]
    }

    proc LogEvent {event section logData} {
        variable np
        variable ns
        
        if {[lindex $logData 2] == 0} {
            lappend logData "unknown"
        } else {
            lappend logData [format_bytes [lindex $logData 2]]
        }
        
        lappend logData [format_bytes [lindex $logData 3]]

        ${np}::sndall $event $section [${np}::ng_format $event $section $logData]        
        
        return 0
    }

    #
    ### END -- ngBot methods
    

    proc format_bytes {amount} {
        foreach dec {0 0 1 2 2 2 2} unit {B KB MB GB TB PB EB} {
            if {abs($amount) >= 1024} {
                set amount [expr {double($amount) / 1024.0}]
            } else {
                break
            }
        }
        
        return [format "%.*f%s" $dec $amount $unit]
    }    

}
