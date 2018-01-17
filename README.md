# libmsgctl
Runtime controllable debug message library.

# How to use with a application:
1. add target definition into target_table in libmsgctl.h, and then re-compile libmsgctl.
                 
        /* one application/thread one target, edit it. */
            :
        #define TARGET_APP2     (2<<16)
        #define TARGET_APP3     (3<<16)
            :
        static target_ctl_t target_table[]={
                     :
                {TARGET_APP2, "app2", "M_PATH2", "M_LVL2"},
                {TARGET_APP3, "app3", "M_PATH3", "M_LVL3"},
                {0, "", "", ""} /* the end */
2. add two lines code that listed below in your source file, use MSGprintf to replace your printf function.
      
          #include <libmsgctl.h>
          #define your_printf(str, args...)       MSGprintf(TARGET_APP1|L_DEBUG, str, ##args) 
3. add -lmsgctl into Makefile of your application and rebuild it.

# How to control:
1. During runtime: use control program "msgctl".

       the usage is:
               msgctl [program_name] [command]
       where
               program_name: specify a name that added into target_table[].
               command:
                       msg_disabled: no message dump.
                       msg_console: messages dump to console.
                       msg_syslog: messages dump to syslog (default setting)
                       msg_to=[filename or device name] :dump to file or other terminal device.
                       level=[0-5] : message level, 0.Debug(default), 1.Info, 2.Warning, 3.Error, 4.Emergency, 5.Quiet.
      ex:
 
          root> msgctl app1 msg_console   <---let app1's message dump to console.
          root> msgctl app2 msg_to=/dev/pts/1 <---let app2's message dump to terminal pts/1.
 2. When initial time: use environment variable.
 
      ex:
      
          root> M_PATH1=msg_console ./app1   <---set message output to console.
          root> M_LVL=3 ./app1               <---set message level to Error (Error and Emergency messages shown only).
