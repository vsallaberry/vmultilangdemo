--
-- ada AdaTest2 Package, called by C, and calling a c utililty function.
--
with ADA.Text_IO;
use  ADA.Text_IO;
with ADA.Integer_Text_IO;
use  ADA.Integer_Text_IO;
with GNAT.Source_Info;

package body AdaTest2 is
    function AdaTest2_Fun return Integer is
        value : Integer;
    begin
        Put_Line("    + [" & GNAT.Source_Info.Source_Location & "] Hello from ada2");
        value := AdaTest2_C_Util;
        Put("    + " & GNAT.Source_Info.Source_Location & "] ada2: c_util result:");
        Put(value);
        Put_Line("");
        return value;
    end AdaTest2_Fun;
end AdaTest2;
