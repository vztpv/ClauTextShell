# Using VisualStudio2017, Modern? C++, Windows10

# ClauTextShell  with https://github.com/vztpv/ClauText 

# Command
  1. $print    # print all data and events (, and Main).
  2. $exit
  3. $M
  4. $cls      # clear screen

# Command not to start with '$'
  it is considered data or events (, or Main).
  ex)
  << : x = 1 y = {
  << :         z = 0 
  << : }
  >> : Data Added!
  << : $print
  >> : global
  x = 1 y =  { z = 0 }
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
  8. Find : f
  9. number 1, 2 <- do! for many same name...
  
# ToDo
  1. $call=id
  2. $load="input.txt"
  3. $save="output.txt"
  4. $save_event_only="output2.txt" # include main(when main exist)
  5. $save_data_only="output3.txt"
