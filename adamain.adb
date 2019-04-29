--
-- Copyright (C) 2019 Vincent Sallaberry
--  vmultilangdemo <https://github.com/vsallaberry/vmultilangdemo>
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License along
-- with this program; if not, write to the Free Software Foundation, Inc.,
-- 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
-- ----------------------------------------------------------------------
-- ada AdaMain Program.
--
with    ADA.Text_IO,
        ADA.Command_line,
        ADA.Integer_Text_IO,
        GNAT.Source_info,
        Interfaces.C,
        Interfaces.C.Strings,
        AdaMain_CCMain;
use     ADA.Text_IO;

procedure AdaMain is
    c_argc  : constant Integer := ADA.Command_Line.Argument_Count;
    c_argv  : array(0..c_argc) of Interfaces.C.Strings.chars_ptr;
begin
    Put_Line("+ [" & GNAT.Source_Info.Source_Location & "] Hello from ada. Arguments:");
    c_argv(0) := Interfaces.C.Strings.New_Char_Array(Interfaces.C.To_C("ADAMAIN"));
    for I in 1..c_argc loop
        ADA.Text_IO.Put("  ");
        ADA.Integer_Text_IO.Put(I);
        ADA.Text_IO.Put_Line(") " & ADA.Command_line.Argument(I));
        c_argv(I) := Interfaces.C.Strings.New_Char_Array(Interfaces.C.To_C(ADA.Command_line.Argument(I)));
    end loop;
    AdaMain_CCMain.cc_main(c_argc + 1, c_argv'address);
end AdaMain;

