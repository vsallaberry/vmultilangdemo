/*
 * Copyright (C) 2018-2019 Vincent Sallaberry
 * vmultilangdemo <https://github.com/vsallaberry/vmultilangdemo>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * -------------------------------------------------------------------------
 * JMain class with main entry and native method nat_main
 */
//import Build;
public class JMain {
    public static final int JMAIN_VER = 1;
    public static native int nat_main();
    public static void main(String[] args) {
        System.out.println("  + [JMain::main] Hello from java JMain::main");
        for (String s : args)
            System.out.println("  + [JMain::main]  arg: " + s);
        System.out.println("  + [JMain::main]  version : " + Build.APP_VERSION);
        System.out.println("  + [JMain::main]  git     : " + Build.BUILD_GITREV);
        //nat_main();
    }
    public static int jmain(String[] args) {
        System.out.println("  + [JMain::jmain] Hello from java JMain::jmain");
        main(args);
        System.out.println("  + [JMain::jmain] calling nat_main()");
        return nat_main();
    }
}
