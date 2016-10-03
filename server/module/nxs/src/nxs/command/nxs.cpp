#include <nxs/core/nxs.hpp>
#include <nxs/core/nex.hpp>
#include <nxs/command.hpp>
#include <nxs/network/request.hpp>
#include <nxs/database.hpp>
#include <iostream>

namespace nxs
{
    template<>
    struct commands<command::nxs>
    {
        static void version(nxs::nex& nex)
        {
            nex.output().add(nxs::version());
        }

        static void test(nxs::nex& nex)
        {
            //nex.output().file_add("d:/ads.txt");

            /*
            std::string result = R"__(
            <form enctype="multipart/form-data" action="/nxs::test;;" method="post">
                <input id="file" type="file" />
                <input type = "submit" />
            </form>
            )__";*/

            std::string result = R"__(
            <h1>Projet nex</h1>
            Ce projet a pour but de cr�er un r�seau de donn�es structur� et de fournir des fonctionnalit�s modulable afin que l'utilisateur puisse g�rer depuis une interface client g�n�rique et personnalisable toute donn�e qu'elle soit local ou r�seau.
            <br />
            <br /><img src = 'http://nex.neuroshok.com/image/nex_presentation.jpg' />
            )__";
            nex.output().add(result);
        }

        static void dbtest(nxs::nex& nex)
        {

        }
    };

    template<>
    void command::init<command::nxs>()
    {
        command& version = nxs::command::add("nxs", "version", &nxs::commands<command::nxs>::version);
        version.help_set("return nxs version");

        command& test = nxs::command::add("nxs", "test", &nxs::commands<command::nxs>::test);
        test.param_add("zeta", param::optional);
        test.help_set("coucou");
        //test.header_add<headers::data_target>(headers::data_target::memory);

        command& dbtest = nxs::command::add("nxs", "dbtest", &nxs::commands<command::nxs>::dbtest);
        dbtest.param_add("name");
    }
} // nxs
