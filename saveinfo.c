/*****************************************************************************

      SAVEINFO.C - Routine to write out argtool info to the converted file 

      Copyright (C) 1993 - 2021 by Joseph B. Attili, aa2il AT arrl DOT net

******************************************************************************

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

*****************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "argtool.h"

extern void ftn_format_line();
extern int num_entries;
extern struct table_entry entries[] ;

save_argtool_info(fp2,lang,fname,usage,help_msg)

   FILE  *fp2;          /* Output file pointer */
   int   lang;          /* Language */
   char  *fname;        /* Program name */
   char  *usage;        /* Usage statement for program */
   char  *help_msg;     /* Help message for program */

{
   int   i,n,k;
   char  *s;

   /* Add leading info */
   if(lang == C_LANG) {

      fprintf(fp2,"\nchar *PROGRAM_NAME=\"%s\";\n",fname);
#if 0
      fprintf(fp2,"char *USAGE=\"%s\";\n",usage);
#else
      fprintf(fp2,"char *USAGE;\n");
      fprintf(fp2,"char *USAGE1=\"%s\";\n",usage);
      remove_cr(help_msg);
#endif
      fprintf(fp2,"char *HELP_MSG=\"%s\";\n",help_msg);
      fprintf(fp2,"#include \"argtool.h\"\n");
      fprintf(fp2,"int arg_entries=%d;\n",num_entries);
      fprintf(fp2,"struct table_entry arg_table[%d] = {\n",num_entries);

   } else if(lang == FTN_LANG) {

      fprintf(fp2,"\n      character PROGRAM_NAME*%d\n      common/program_name_stuff/program_name\n",strlen(fname)+1);
      s=detab(usage);
      strcpy(usage,s);
      free(s);
      fprintf(fp2,"      character USAGE*%d\n      common/usage_stuff/usage\n",strlen(usage)+1);
      fprintf(fp2,"      character HELP_MSG*%d\n      common/help_msg_stuff/help_msg\n",strlen(help_msg)+1);

#ifdef vms
      fprintf(fp2,"\n      include 'argpath:argtool.inc'\n");
#endif
#if defined(sun) || defined(mips)  || defined(__PARAGON__) || defined(linux)
      fprintf(fp2,"\n      include 'argtool.inc'\n");
/*      fprintf(fp2,"\n      include '$ARGPATH/argtool.inc'\n");*/
#endif
#ifdef MSDOS
      fprintf(fp2,"\n      include 'argtool.inc'\n");
#endif

/*      fprintf(fp2,"      integer*4 arg_entries\n");
      fprintf(fp2,"      record/table_entry/arg_table(0:%d)\n",num_entries);
      fprintf(fp2,"      common/arg_stuff/arg_entries,arg_table\n\n");   */
      ftn_format_line(fp2,"PROGRAM_NAME='%s'//char(0)",fname);
      ftn_format_line(fp2,"USAGE='%s'//char(0)",usage);
      ftn_format_line(fp2,"HELP_MSG='%s'//char(0)",help_msg);
      fprintf(fp2,"      arg_entries=%d\n",num_entries);

   } else
      fatal_error(1,"Fatal Error - unknown language code %d\n",lang);

   /* Add table that defines associations in proc'ed file */
   for(n=0; n<num_entries; n++) {

      if(lang == C_LANG) {
         fprintf(fp2,"{ \"%s\" , \"%s\" , \"%s\" , \"%s\" , ",
         entries[n].uname,entries[n].vartype,entries[n].varname,entries[n].format);

         /* Don't include quotes on init values */
         if( entries[n].initvalue ) 
            k=strlen(entries[n].initvalue)-1;
          else
            k=0;
         if( k>0 && entries[n].initvalue[0]=='"' && 
                    entries[n].initvalue[k]=='"')  {
            entries[n].initvalue[k]=0;
            fprintf(fp2,"\"%s\" , ",entries[n].initvalue+1);
            entries[n].initvalue[k]='"';
         } else {
            fprintf(fp2,"\"%s\" , ",entries[n].initvalue);
         }

	 remove_cr(entries[n].comment);
         fprintf(fp2," \" %s \" , %d , %d , %d , %d , %d , ",entries[n].comment,
                  entries[n].flag,entries[n].required,entries[n].repeated,
                  entries[n].flagged,entries[n].param_number);

         /* Generate string for address inits */
         if( entries[n].flag ) {
            fprintf(fp2," 0 ,");
         } else {
            fprintf(fp2,"(char *)&%s ,",entries[n].varname);
         }

         fprintf(fp2," %d , %d , %d }",entries[n].occurances,entries[n].pointer,entries[n].size);
         if(n<num_entries-1) fprintf(fp2," ,");

         fprintf(fp2,"\n");

      } else if(lang == FTN_LANG) {

         fprintf(fp2,"\n");
         fprintf(fp2,"      call set_table_ftn(arg_table,%d,\n",n);
	 fprintf(fp2,"     &'%s'//char(0),\n",entries[n].uname);
         fprintf(fp2,"     &'%s'//char(0),\n",entries[n].vartype);
         fprintf(fp2,"     &'%s'//char(0),\n",entries[n].varname);
         fprintf(fp2,"     &'%s'//char(0),\n",entries[n].format);

         /* Don't include quotes on init values */
         if( entries[n].initvalue ) 
            k=strlen(entries[n].initvalue)-1;
          else
            k=0;
         if( k>0 && entries[n].initvalue[0]=='\'' && 
                    entries[n].initvalue[k]=='\'')  {
            entries[n].initvalue[k]=0;
            if( strlen( entries[n].initvalue+1 ) > 0 ) 
	      fprintf(fp2,"     &'%s'//char(0),\n",entries[n].initvalue+1);
            else
	      fprintf(fp2,"    &char(0),\n");
            entries[n].initvalue[k]='\'';
         } else {
            if( k > 0 )
               fprintf(fp2,"     &'%s'//char(0),\n",entries[n].initvalue);
            else
               fprintf(fp2,"     &char(0),\n");
         }

/*         ftn_format_line(fp2,"arg_table(%d).comment='%s'//char(0)",n,entries[n].comment); */
	 fprintf(fp2,"     &'%s'//char(0),\n",entries[n].comment);

         fprintf(fp2,"     &%d,%d,%d,%d,%d,\n",
                                entries[n].flag,entries[n].required,
                                entries[n].repeated,entries[n].param_number,
                                entries[n].flagged);

         /* Generate string for address inits */
         if( entries[n].flag ) {
            fprintf(fp2,"     &%d,",0);
         } else {
#ifdef vms
            fprintf(fp2,"     &%%loc(%s),",entries[n].varname);
#else
            fprintf(fp2,"     &loc(%s),",entries[n].varname);
#endif
         }

         fprintf(fp2,"%d,%d,%d)\n",entries[n].occurances,
                            entries[n].pointer,entries[n].size);

      }

   }
   if(lang == C_LANG) 
      fprintf(fp2,"};\n");

}



int remove_cr(char *s)
{
  int i;
  for(i=0;i<strlen(s);i++) {
    if(s[i]<' ')
      s[i]=' ';
  }
}
