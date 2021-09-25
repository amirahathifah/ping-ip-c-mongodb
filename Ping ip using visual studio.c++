#include <iostream>
#include <fstream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include "curl/curl.h"
#include <windows.h>

using namespace std;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::finalize;

int main(void)
{
		mongocxx::instance inst{};
		mongocxx::client conn{ mongocxx::uri{"mongodb://admin:TempPassword@database.gongetz.com:27017/gngoffice?authSource=admin"} };
		mongocxx::database db = conn["gngoffice"];
		mongocxx::collection coll = db["gng_assets"];

		int count = 1;
		while (true)
		{

			//create query
			auto filter = document{} << "assetsstatus" << "ACTIVE" << finalize;

			// Create the find options with the projection
			mongocxx::options::find opts{};
			opts.projection(document{} << "_id" << 0 << "assetsstatus" << 0 << finalize);

			// Execute find with options
			mongocxx::cursor cursor = coll.find(filter.view(), opts);

			for (auto&& doc : cursor)
			{

				//std::cout << "" << doc["assetsip"].get_utf8().value.to_string() << "\n";
				string s = "ping -c1 -s1 " + doc["assetsip"].get_utf8().value.to_string() + "";
				const char* ping = s.c_str();
				int x = system(ping);

				if (x == 0)
				{
					cout << "success" << endl;
					Sleep(5000); // wait 5 second
				}
				else
				{
					cout << "failed" << endl;

					std::ofstream example("example.txt");

					// Always check to see if the file is open and for errors.
					if (example.is_open())
					{
						// If it is open we can do our writing to the file.
						// Here is a example of this.
						example << doc["assetsip"].get_utf8().value.to_string() << " IP FAILED!";
					}
					else
					{
						// If the file isn't open something went wrong. Point that out.
						std::cout << "Something went wrong with opening the file!";
					}

					// After you are done with the file always close it.
					example.close();

					string c = "curl --ssl-reqd --url smtps://smtp.gmail.com:465 --user azaharmyra95@gmail.com:Qwertyuiop@2395 --mail-from azaharmyra95@gmail.com --mail-rcpt mirapsm95@gmail.com --upload-file example.txt";

					const char* e = c.c_str();
					int y = system(e);
					Sleep(5000); // wait 5 second


				}

			}

			//if (count++)
			//{
			//	break;
			//}

		}

}