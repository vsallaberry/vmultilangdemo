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
-- ada AdaTest2 Package, called by C, and calling a c utililty function.
--
with ADA.Text_IO;
use  ADA.Text_IO;
with ADA.Integer_Text_IO;
use  ADA.Integer_Text_IO;
with GNAT.Source_Info;

package body AdaTest2 is
    function AdaTest2_Fun(i : in Integer) return Integer is
        value : Integer;
    begin
        Put_Line("    + [" & GNAT.Source_Info.Source_Location & "] Hello from ada2");
        value := AdaTest2_C_Util(i);
        Put("    + [" & GNAT.Source_Info.Source_Location & "] ada2: c_util result:");
        Put(value);
        Put_Line("");
        return value;
    end AdaTest2_Fun;
end AdaTest2;
