pragma Ada_2005;
pragma Style_Checks (Off);

with Interfaces.C; use Interfaces.C;

package a_h is

   function some_c_decl (arg1 : int) return int;  -- a.h:1
   pragma Import (C, some_c_decl, "some_c_decl");

end a_h;
