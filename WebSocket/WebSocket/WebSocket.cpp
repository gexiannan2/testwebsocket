#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/roles/client_endpoint.hpp>
#include <iostream>
#include <map>
#include <functional>


typedef websocketpp::server<websocketpp::config::asio> websocket_server;
typedef websocketpp::client<websocketpp::config::asio> websocket_client;

class relay_server {
public:
	relay_server() {
		// 初始化服务器
		server.init_asio();
		server.set_reuse_addr(true);
		server.set_message_handler(bind(&relay_server::on_message, this, std::placeholders::_1, std::placeholders::_2));

		// 初始化客户端
		client.init_asio();
		client.set_open_handler(bind(&relay_server::on_open, this, std::placeholders::_1));
		client.set_message_handler(bind(&relay_server::on_client_message, this, std::placeholders::_1, std::placeholders::_2));
	}

	void on_message(websocketpp::connection_hdl hdl, websocket_server::message_ptr msg) {
		std::string payload = msg->get_payload();
		std::cout << "Received message from web client: " << payload << std::endl;

		websocketpp::lib::error_code ec;
		websocket_client::connection_ptr con = client.get_connection("ws://another-service-url", ec);
		if (ec) {
			std::cout << "Connect failed: " << ec.message() << std::endl;
			return;
		}

		connection_map[con] = hdl;
		client.connect(con);
	}

	void on_open(websocketpp::connection_hdl hdl) {
		// 可以在这里初始化与另一个服务的连接，如发送认证信息等
	}

	void on_client_message(websocketpp::connection_hdl hdl, websocket_client::message_ptr msg) {
		std::string payload = msg->get_payload();
		std::cout << "Received message from another service: " << payload << std::endl;

		// 转发消息到原始的网页客户端
		websocketpp::connection_hdl original_hdl = connection_map[hdl];
		server.send(original_hdl, payload, websocketpp::frame::opcode::text);
	}

	void start_server(uint16_t port) {
		server.listen(port);
		server.start_accept();
		std::cout << "Server listening on port " << port << std::endl;
		server.run();
	}

private:
	websocket_server server;
	websocket_client client;
	std::map<websocketpp::connection_hdl, websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connection_map;
};

int main() {
	relay_server server;
	server.start_server(9002);
	return 0;
}


