export module tm.clients.ps.log_battle_messages;

import tm.clients.ps.room_message_block;

import tm.open_file;

import std_module;

namespace technicalmachine::ps {

export auto log_battle_messages(std::filesystem::path const & directory, RoomMessageBlock const block) -> void {
	auto file = open_text_file(directory / std::filesystem::path(block.room()) / "server_messages.txt");
	file << '>' << block.room() << '\n';
	file << block.str() << '\n' << std::flush;
}

} // namespace technicalmachine::ps
