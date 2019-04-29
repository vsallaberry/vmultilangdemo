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
-- ada AdaTest2 Package declaration
--   + called by C (Export AdaTest2_Fun)
--   + calling a c utililty function (Import AdaTest2_C_Util)
--
package AdaTest2 is
    function AdaTest2_Fun(i : in Integer) return Integer;
    function AdaTest2_C_Util(i : in Integer) return Integer;
    pragma Export(C, AdaTest2_Fun, "ada_adatest2");
    pragma Import(C, AdaTest2_C_Util, "c_util_for_ada2");
end AdaTest2;
