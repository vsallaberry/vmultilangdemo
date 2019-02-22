--
-- ada AdaTest Procedure
--   + called by C (Export AdaTest)
--
procedure AdaTest;
pragma Export(C, AdaTest, "ada_adatest");

