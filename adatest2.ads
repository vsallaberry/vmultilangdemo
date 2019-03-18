--
-- ada AdaTest2 Package
--   + called by C (Export AdaTest2_Fun)
--   + calling a c utililty function (Import AdaTest2_C_Util)
--
package AdaTest2 is
    function AdaTest2_Fun(i : in Integer) return Integer;
    function AdaTest2_C_Util(i : in Integer) return Integer;
    pragma Export(C, AdaTest2_Fun, "ada_adatest2");
    pragma Import(C, AdaTest2_C_Util, "c_util_for_ada2");
end AdaTest2;
