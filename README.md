# Using VisualStudio2017(C++17option?), Modern? C++17, Windows10

# ClauTextShell  with https://github.com/vztpv/ClauText 
    1. using "wiz" folder in ClauText Project.
# Command
     1. $print    # print all data and events (, and Main).
     2. $exit
     3. $M
     4. $cls      # clear screen
     5. $call=id
     6. $load="input.txt"
     7. $save="output.txt"
     8. $save_event_only="output2.txt" # no include main(when main exist)
     9. $save_data_only="output3.txt"
    10. $print_data_only
# Command not to start with '$'
    it is considered data or events (, or Main).
    ex)
    << : x = 1 y = {
    << :         z = 0 
    << : }
    >> : Data Added!
    << : $print
    >> : global
    x = 1 y = { z = 0 }
    << : 

# $M Usage ( ClauText includes editor? https://github.com/vztpv/Console-Data-File-Editor )
    1. Up : w or W
    2. Down : s or S

    3. Enter : Enter key

    4. Back : b
    5. Quit : q

    6. Editor mode : e 
        ( add - a, change - c, remove - r, save - s )
    7. Text mode : t ( only at main screen, first screen )
    8. Find Line : f ( line : integer, start at zero. )
    9. Number 1, 2 <- do! for many same names, 1 : first of many same names, 2 : last of many same names
  
# ToDo And DONE
    DONE - 1. $call=id
    DONE - 2. $load="input.txt"
    DONE - 3. $save="output.txt"
    DONE - 4. $save_event_only="output2.txt" # no include main(when main exist)
    DONE - 5. $save_data_only="output3.txt"
    6. Main check! ( num of Main is 0. ) ??
    DONE - 7. $print_data_only
