--
-- ada AdaTest Procedure called by C.
--
with ADA.Text_IO; use ADA.Text_IO;
with GNAT.Source_info;

procedure AdaTest is begin
    Put_Line("    + [" & GNAT.Source_Info.Source_Location & "] Hello from ada");
end AdaTest;

