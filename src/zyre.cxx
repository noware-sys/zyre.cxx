#pragma once

#include "zyre.hxx"

#include <cerrno>
#include <signal.h>

#include <boost/function_equal.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <zmq/msg.cxx>

#include <zpoller.h>

zyre::zyre (void)
//	: gentor ()
{
	_running = false;
	_zyre = nullptr;
	_reception = nullptr;
	//_exoreception = nullptr;
}

zyre::~zyre (void)
{
	std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::~zyre()::in scope::pre" << std::endl;
	//stop ();
	fin ();
	std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::~zyre()::in scope::post" << std::endl;
	//zclock_sleep (300);
}

void zyre::s_signal_handler (int/* signal_value*/)
{
	int rc;
	
	rc = write (s_fd, "S_STOP", sizeof ("S_STOP"));
	
	if (rc != sizeof ("S_STOP"))
	{
		write (STDOUT_FILENO, "Error while writing to self-pipe", sizeof ("Error while writing to self-pipe") - 1);
		
		//exit EXIT_FAILURE;
	}
}

void zyre::s_catch_signal (int fd)
{
	s_fd = fd;
	
	struct sigaction action;
	action.sa_handler = (void *) &(zyre::s_signal_handler);
	
	// It doesn't matter if SA_RESTART is set, because the self-pipe will wake up zmq_poll,
	// but setting to 0 will allow zmq_read to be interrupted.
	action.sa_flags = 0;
	sigemptyset (&action.sa_mask);
	sigaction (SIGINT, &action, nullptr);
	sigaction (SIGTERM, &action, nullptr);
}

const bool zyre::init (void)
{
	//if (!inited ())
	//{
		if (_zyre == nullptr)
			_zyre = zyre_new (nullptr);
		
		//_running = true;
		//_id = id ();
		
		if (_reception == nullptr)
			_reception = new boost::thread (boost::bind (boost::mem_fn (&zyre::receive), this));
	//}
	
	//return true;
	return inited ();
}

const bool zyre::inited (void) const
{
	//std::cerr << std::endl << "_zyre==[" << _zyre << ']' << std::endl;
	//return _zyre != nullptr;
	std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::inited()::_zyre == nullptr==[" << (_zyre == nullptr) << "]" << std::endl;
	std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::inited()::_reception == nullptr==[" << (_reception == nullptr) << "]" << std::endl;
	return _zyre != nullptr && _reception != nullptr;
}

const bool zyre::fin (void)
{
	//if (!stop ())
	//	return false;
	stop ();
	
	//if (reception_is_set ())
	//	reception_unset ();
	
	
	/*
	std::cerr << "zyre::fin()::_reception::_running=false" << std::endl;
	_running = false;
	std::cerr << "zyre::fin()::_reception::joining" << std::endl;
	_reception -> join ();
	std::cerr << "zyre::fin()::_reception::joined" << std::endl;
	*/
	
	/*
	std::cerr << "zyre::finalize()::zyre_destroy()::pre" << std::endl;
	if (_zyre != nullptr)
	{
		std::cerr << "zyre::finalize()::zyre_destroy()::destroying" << std::endl;
		//stop ();
		zyre_stop (_zyre);
		zyre_destroy (&_zyre);
		std::cerr << "zyre::finalize()::zyre_destroy()::destroyed" << std::endl;
		_zyre = nullptr;
		_running = false;
		//_id = "";
	}
	*/
	
	std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::_reception::deletion" << std::endl;
	if (_reception != nullptr)
	{
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::_reception->interrupting()ing" << std::endl;
		_reception -> interrupt ();
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::_reception->interrupting()ed" << std::endl;
		
		//std::cerr << "zyre::finalize()::_reception->loccast()ing" << std::endl;
		//assert (loccast (zmq::msg ("shutdown.dummy")));
		//std::cerr << "zyre::finalize()::_reception->loccast()ed" << std::endl;
		
		/*
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::zyre_stop()ping" << std::endl;
		//stop ();
		zyre_stop (_zyre);
		_running = false;
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::zyre_stop()ped" << std::endl;
		*/
		
		//zsock_t * zyre_socket_ = zyre_socket (_zyre);
		//zsock_destroy (&zyre_socket_);
		//zpoller_destroy (&poller);
		
		s_signal_handler (1);
		/*
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::zyre_destroy()ing" << std::endl;
		zyre_destroy (&_zyre);
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::zyre_destroy()ed" << std::endl;
		_zyre = nullptr;
		*/
		
		//reception_unset ();
		
		//try
		//{
			std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::_reception->join()ing" << std::endl;
			_reception -> join ();
			std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::_reception->join()ed" << std::endl;
		/*}
		catch (...)
		{
			std::cerr << "zyre::finalize()::_reception->join()ing::caught an exception" << std::endl;
		}*/
		
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::_reception::delete-ing" << std::endl;
		delete _reception;
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::finalize()::_reception::delete-ed" << std::endl;
		_reception = nullptr;
		
		//_id = "";
	}
	
	//zclock_sleep (300);
	//_id = "";
	//reception_unset ();
	
	std::cerr << "zyre::finalize()::zyre_destroy()::pre" << std::endl;
	if (_zyre != nullptr)
	{
		std::cerr << "zyre::finalize()::zyre_destroy()::destroying" << std::endl;
		//stop ();
		//zyre_stop (_zyre);
		zyre_destroy (&_zyre);
		std::cerr << "zyre::finalize()::zyre_destroy()::destroyed" << std::endl;
		_zyre = nullptr;
		//_running = false;
	}
	
	
	//stop ();
	reception_unset ();
	
	return _zyre == nullptr && _reception == nullptr;
}

const bool zyre::running (void) const
{
	std::cerr << "zyre::running()::inited()==[" << inited () << "]" << std::endl;
	if (!inited ())
		return false;
	
	std::cerr << "zyre::running()::_running==[" << _running << "]" << std::endl;
	return _running;
	//return true;
}

const bool zyre::stop (void)
{
	if (!inited ())
		return true;
	
	std::cerr << "zyre::stop()::called" << std::endl;
	if (_running)
	{
		std::cerr << "node::stop()::stopping zyre node" << std::endl;
		zyre_stop (_zyre);
		std::cerr << "node::stop()::stopped zyre node" << std::endl;
		
		_running = false;
	}
	
	return true;
	//return !_running;
}

const bool zyre::start (void)
{
	if (!inited ())
		return false;
	
	if (!_running)
	{
		zyre_start (_zyre);
		
		//zyre_set_interface (_zyre, "enp2s0");
		zyre_set_verbose (_zyre);
		
		//std::cerr << "Started Zyre Node" << std::endl;
		//_id = id ();
		_running = true;
	}
	
	return true;
	//return _running;
}

const std::string zyre::id (void) const
{
	return zyre_uuid (_zyre);
}

const bool zyre::leave (const std::string & group)
{
	if (/*!inited () || */!running ())
		//return true;
		return false;
	
	return zyre_leave (_zyre, group.c_str ()) == 0;
	
	//return true;
}

const bool zyre::joined (const std::string & group) const
{
	if (!running ())
		return false;
	
	std::map <const unsigned int, const std::string> grps;
	
	grps = groups_own ();
	
	for (const std::pair <const unsigned int, const std::string> & grp : grps)
	{
		if (grp.second == group)
			return true;
	}
	
	return false;
}

const bool zyre::join (const std::string & group)
{
	if (/*!inited () || */!running ())
		return false;
	
	return zyre_join (_zyre, group.c_str ()) == 0;
}

const std::map <const unsigned int, const std::string> zyre::groups_own (void) const
{
	std::map <const unsigned int, const std::string> groups;
	
	//if (/*!inited () || */!running ())
	//	return groups;
	
	zlist_t * groups_list;
	void * group;
	unsigned int ndx;
	
	groups_list = zyre_own_groups (_zyre);
	
	if (groups_list == nullptr)
		return groups;
	
	for (group = zlist_first (groups_list), ndx = 1; group != nullptr; group = zlist_next (groups_list), ++ndx)
	{
		groups [ndx] = static_cast <const char *> (group);
		//groups.insert_or_assign (ndx, std::string (static_cast <const char *> (group)));
	}
	
	zlist_destroy (&groups_list);
	return groups;
}

const std::map <const unsigned int, const std::string> zyre::groups_peers (void) const
{
	std::map <const unsigned int, const std::string> groups;
	
	if (/*!inited () || */!running ())
		return groups;
	
	zlist_t * groups_list;
	void * group;
	unsigned int ndx;
	
	groups_list = zyre_peer_groups (_zyre);
	
	if (groups_list == nullptr)
		return groups;
	
	for (group = zlist_first (groups_list), ndx = 1; group != nullptr; group = zlist_next (groups_list), ++ndx)
	{
		groups [ndx] = static_cast <const char *> (group);
		//groups.insert_or_assign (ndx, std::string (static_cast <const char *> (group)));
	}
	
	zlist_destroy (&groups_list);
	return groups;
}

const std::map <const unsigned int, const std::string> zyre::peers (void) const
{
	std::map <const unsigned int, const std::string> peers;
	
	if (/*!inited () || */!running ())
		return peers;
	
	zlist_t * peers_list;
	void * peer;
	unsigned int ndx;
	
	peers_list = zyre_peers (_zyre);
	
	if (peers_list == nullptr)
		return peers;
	
	for (peer = zlist_first (peers_list), ndx = 1; peer != nullptr; peer = zlist_next (peers_list), ++ndx)
	{
		std::cerr << " zyre::peers(void)::index::[" << ndx << "]" << std::endl;
		peers [ndx] = static_cast <const char *> (peer);
		//peers.insert_or_assign (ndx, std::string (static_cast <const char *> (peer)));
		std::cerr << " zyre::peers(void)::peers[index]::[" << peers [ndx] << "]" << std::endl;
	}
	
	zlist_destroy (&peers_list);
	return peers;
}

const std::map <const unsigned int, const std::string> zyre::peers (const std::string & group) const
{
	std::map <const unsigned int, const std::string> peers;
	
	if (/*!inited () || */!running ())
		return peers;
	
	zlist_t * peers_list;
	void * peer;
	unsigned int ndx;
	
	peers_list = zyre_peers_by_group (_zyre, group.c_str ());
	
	if (peers_list == nullptr)
		return peers;
	
	for (peer = zlist_first (peers_list), ndx = 1; peer != nullptr; peer = zlist_next (peers_list), ++ndx)
	{
		std::cerr << " zyre::peers(group=[" << group << "])::index::[" << ndx << "]" << std::endl;
		peers [ndx] = static_cast <const char *> (peer);
		//peers.insert_or_assign (ndx, std::string (static_cast <const char *> (peer)));
		std::cerr << " zyre::peers(group=[" << group << "])::peers[index]::[" << peers [ndx] << "]" << std::endl;
	}
	
	zlist_destroy (&peers_list);
	return peers;
}

const bool zyre::loccast (const zmq::msg & msg) const
{
	return unicast (msg, id ());
}

const bool zyre::unicast (const zmq::msg & msg, const std::string & peer/* peer id*/) const
//const bool zyre::unicast (const zmsg_t * zmsg, const std::string & peer/* peer id*/) const
{
	//assert (running ());
	if (/*!inited () || */!running ())
		return false;
	
	//zmsg_t * msg = zmsg_new ();
	//zmsg_addstr (msg, message.c_str ());
	
	//  0 == success
	// -1 == failure
	
	
	zmsg_t * zmsg;
	
	//zmsg = (zmsg_t *) msg;
	zmsg = msg.operator const zmsg_t * ();
	
	return zyre_whisper (_zyre, peer.c_str (), &zmsg) == 0;
	
	//return true;
}

const std::string zyre::any (void) const
{
	std::map <const unsigned int, const std::string> peers_list;
	//std::map <const unsigned int, const std::string> peers_list;
	//noware::nr peers_list_size;
	unsigned int peers_list_size;
	unsigned int index;	// Optional
	std::string peer;	// Optional
	
	peers_list = peers ();
	peers_list_size = peers_list.size ();
	
	std::cerr << " zyre::any(void)::peers_list_size::[" << peers_list_size << "]" << std::endl;
	
	if (/*(unsigned int) */peers_list_size < 1)
		return "";
	
	boost::random::random_device randev;
	boost::random::uniform_int_distribution <unsigned int> distr (1, peers_list_size);
	
	//return peers_list [distr (randev)];
	index = distr (randev);
	std::cerr << " zyre::any(void)::peers_list[index]::[" << index << "]" << std::endl;
	peer = peers_list [index];
	std::cerr << " zyre::any(void)::peer::[" << peer << "]" << std::endl;
	return peer;
}

const std::string zyre::any (const std::string & group) const
{
	std::map <const unsigned int, const std::string> peers_list;
	//std::map <const unsigned int, const std::string> peers_list;
	//noware::nr peers_list_size;
	unsigned int peers_list_size;
	unsigned int index;	// Optional
	std::string peer;	// Optional
	
	peers_list = peers (group);
	peers_list_size = peers_list.size ();
	
	std::cerr << " zyre::any(group=[" << group << "])::peers_list_size::[" << peers_list_size << "]" << std::endl;
	
	if (/*(unsigned int) */peers_list_size < 1)
		return "";
	
	boost::random::random_device randev;
	boost::random::uniform_int_distribution <unsigned int> distr (1, peers_list_size);
	
	//return peers_list [distr (randev)];
	index = distr (randev);
	std::cerr << " zyre::any(group=[" << group << "])::peers_list[index]::[" << index << "]" << std::endl;
	peer = peers_list [index];
	std::cerr << " zyre::any(group=[" << group << "])::peer::[" << peer << "]" << std::endl;
	return peer;
}

const bool zyre::anycast (const zmq::msg & msg, std::string & peer_id) const
{
	/*
	std::map <const unsigned int, const std::string> peers_list;
	unsigned int peers_list_size;
	
	peers_list = peers ();
	peers_list_size = peers_list.size ();
	
	if ((unsigned int) peers_list_size < 1)
		return false;
	
	boost::random::random_device randev;
	boost::random::uniform_int_distribution <unsigned int> distr (1, peers_list_size);
	
	peer_id = peers_list [distr (randev)];
	
	return unicast (msg, peer_id);
	*/
	peer_id = any ();
	
	if (peer_id.empty ())
		return false;
	
	return unicast (msg, peer_id);
}

const bool zyre::anycast (const zmq::msg & msg, std::string & peer_id, const std::string & group) const
{
	/*
	std::map <const unsigned int, const std::string> peers_list;
	//unsigned int peers_list_size;
	unsigned int peers_list_size;
	
	peers_list = peers (group);
	peers_list_size = peers_list.size ();
	
	//if ((unsigned int) peers_list_size < 1)
	if (peers_list_size < 1)
	{
		std::cerr << "zyre::anycast()::peers_list_size < 1::true" << std::endl;
		return false;
	}
	std::cerr << "zyre::anycast()::peers_list_size < 1::false" << std::endl;
	
	boost::random::random_device randev;
	boost::random::uniform_int_distribution <unsigned int> distr (1, peers_list_size);
	
	std::cerr << "zyre::anycast()::return" << std::endl;
	return unicast (msg, peers_list [distr (randev)]);
	*/
	peer_id = any (group);
	
	if (peer_id.empty ())
		return false;
	
	return unicast (msg, peer_id);
}

const bool zyre::multicast (const zmq::msg & msg, const std::string & group) const
//const bool zyre::multicast (const zmsg_t * zmsg, const std::string & group) const
{
	std::cerr << "zyre::multicast()::called" << std::endl;
	
	if (/*!inited () || */!running ())
		return false;
	
//	zmsg_t * msg = zmsg_new ();
	//std::cerr << "zyre::multicast()::2" << std::endl;
//	zmsg_addstr (msg, message.c_str ());
	//zmsg_addstr (msg, "Hello, World.");
	
	signed short int result;
	//std::cerr << "zyre::multicast()::3" << std::endl;
//	std::cerr << "zyre::multicast::zyre_shout()==" << '[' << (result = zyre_shout (_zyre, group.c_str (), &msg)) << ']' << std::endl;
	//zyre_shout (_zyre, "storage", &msg);
	//zclock_sleep (250);
	
	//std::cerr << "zyre::multicast()::4(last)" << std::endl;
	zmsg_t * zmsg;
	
	//zmsg = (zmsg_t *) msg;
	zmsg = msg.operator const zmsg_t * ();
	/*
	
	//assert (zframe_is (zmsg_first (zmsg)));
	zframe_t * frm;
	//char * cstr;
	
	frm = zmsg_first (zmsg);
	//str = zframe_strdup (frm);
	//std::string str ((char *) zframe_data (frm), zframe_size (frm));
	zframe_data (frm);
	zframe_size (frm);
	//std::cerr << "zyre::multicast::str==[" << str << ']' << std::endl;
	assert (zframe_is (frm));
	//assert (frm -> tag == ZFRAME_TAG);
	*/

	/*
	zmsg_t * zmsg;
	zframe_t * frame_p;
	zframe_t ** frame_pp;
	
	zmsg = zmsg_new ();
	//frame = zframe_new_empty ();
	
	for (const std::pair <const noware::nr, zmq::msg::frame> & _frame : msg.data)
	{
		//zframe_reset (_frame, );
		//frame_p = &((zframe_t) _frame.second);
		frame_p = zframe_new (_frame.second.data.data (), _frame.second.data.size ());
		frame_pp = &frame_p;
		
		assert (zframe_is (*frame_pp));
		//assert ((*frame_pp) -> tag == ZFRAME_TAG);
		
		zmsg_append (zmsg, frame_pp);
	}
	*/
	//return *zmsg;
	//printf ("zyre::multicast::str==[%s]\n", cstr);
	//free (cstr);
//	std::cerr << "zyre::multicast::zyre_shout()..." << std::endl;
	//  0 == success
	// -1 == failure
	//return true;
	//return zyre_shout (_zyre, group.c_str (), &(msg.operator const zmsg_t ())) == 0;
	
	//return zyre_shout (_zyre, group.c_str (), &zmsg) == 0;
	result = zyre_shout (_zyre, group.c_str (), &zmsg);
	std::cerr << "zyre::multicast::zyre_shout()[" << (result == 0 ? "success" : "failure") << "]" << std::endl;
	return result == 0;
}

const bool zyre::broadcast (const zmq::msg & msg) const
{
	std::cerr << "zyre::broadcast()::called" << std::endl;
	
	if (/*!inited () || */!running ())
		return false;
	
	return false;
}

const zyre_t * zyre::operator * (void) const
{
	return _zyre;
}

const unsigned int zyre::peers_size (void) const
{
	//if (/*!inited () || */!running ())
	//	return 0;
	
	unsigned int result;
	zlist_t * peers;
	
	result = 0;
	peers = zyre_peers (_zyre);
	
	if (peers == nullptr)
	{
		std::cerr << "zyre::peers_count::peers==nullptr" << std::endl;
		
		return result;
	}
	
	result = zlist_size (peers);
	std::cerr << "zyre::peers_count::result=zlist_size(peers)==[" << result << ']' << std::endl;
	zlist_destroy (&peers);
	
	return result;
}

const unsigned int zyre::peers_size (const std::string & group) const
{
	//if (/*!inited () || */!running ())
	//	return 0;
	
	unsigned int result;
	zlist_t * peers;
	
	result = 0;
	peers = zyre_peers_by_group (_zyre, group.c_str ());
	
	if (peers == nullptr)
	{
		std::cerr << "zyre::peers_count::peers==nullptr" << std::endl;
		
		return result;
	}
	
	result = zlist_size (peers);
	std::cerr << "zyre::peers_count::result=zlist_size(peers)==[" << result << ']' << std::endl;
	zlist_destroy (&peers);
	
	return result;
}

const bool zyre::reception_is_set (void) const
{
	//return _exoreception != nullptr;
	return !(_exoreception.empty ());
	//return !_exoreception.empty () && _reception != nullptr;
}

const bool zyre::reception_unset (void)
{
	std::cerr << "zyre::reception_unset()::called" << std::endl;
	//if (reception_is_set ())
	//{
		//std::cerr << "zyre::reception_unset()::_reception::deleting" << std::endl;
		//delete _reception;
		//std::cerr << "zyre::reception_unset()::_reception::deleted" << std::endl;
		//_reception = nullptr;
		
		//delete _exoreception;
		std::cerr << "zyre::reception_unset()::_exoreception.clear()" << std::endl;
		_exoreception.clear ();
	//}
	
	return true;
	//return !reception_is_set ();
}

const bool zyre::reception_set (const boost::function <void (const zyre_event_t */* zyre_event*/)>	& exomanager)
{
	// It is the same function
	//if (_exoreception/* self*/ == exomanager/* other*/)
	//if (boost::function_equal <boost::function <void (const zyre_event_t *)>, boost::function <void (const zyre_event_t *)>> (_exoreception, exomanager))
	//if (_exoreception.contains <boost::function <void (const zyre_event_t *)>> (exomanager))
	
/*	
	if (_exoreception.target <boost::function <void (const zyre_event_t *)>> () == exomanager.target <boost::function <void (const zyre_event_t *)>> ())
		return true;
	
	// It is not the same function
	else if (reception_is_set ())
		//return false;
		if (!reception_unset ())
			return false;
	
	// http://www.boost.org/doc/libs/1_63_0/doc/html/function/tutorial.html :
	// "Note that the & isn't really necessary unless you happen to be using Microsoft Visual C++ version 6."
	//_exoreception = &exomanager;
*/
		_exoreception = exomanager;
	
	//_reception = new boost::thread (boost::bind (boost::mem_fn (&zyre::receive), this));
	
	return true;
	//return _exoreception == &exomanager;
	//return reception_is_set ();
}

void zyre::receive (void)
{
	zyre_event_t * event;
	
	/*zmq::pollitem_t pollers [] =
	{
		{receiver, 0, ZMQ_POLLIN, 0}
	};*/
	//zmq_pollitem_t pollers [1];
	zpoller_t * poller;
	void * socket;
	int pipefds [2];
	
	//// A return code.
	//int rc;
	
	
	////pollers [0].socket = zyre_socket (_zyre)/* -> handle*/;
	//pollers [0].socket = zsock_resolve (zyre_socket (_zyre));
	//pollers [0].events = ZMQ_POLLIN;
	
	event = nullptr;
	// Make the self-pipe.
	assert (pipe (pipefds) == 0);
	poller = zpoller_new (nullptr);
	
	assert (poller != nullptr);
	assert (zpoller_add (poller, zyre_socket (_zyre)) == 0);
	assert (zpoller_add (poller, (void *) &(pipefds [0])) == 0);
	
	s_catch_signal (pipefds [1]);
	
	//while (!boost::this_thread::interruption_requested () && poller != nullptr && !zpoller_terminated (poller))
	while (true)
	{
		//try
		//{
			std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::waiting for an event" << std::endl;
			//if (poller != nullptr)
			//{
				//try
				//{
					socket = zpoller_wait (poller, -1);
				//	rc = zmq_poll (pollers, 1, -1);
				/*}
				catch (...)
				{
					std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::zpoller_wait()::caught an exception" << std::endl;
				}*/
				
				//std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::zpoller_wait()::zpoller_terminated()::[" << (zpoller_terminated (poller)) << "]" << std::endl;
				/*
				std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::zpoller_wait()::socket == pipefds [0]::[" << (socket == pipefds [0]) << "]" << std::endl;
				std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::zpoller_wait()::socket == &(pipefds [0])::[" << (socket == &(pipefds [0])) << "]" << std::endl;
				std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::zpoller_wait()::socket == zyre_socket(_zyre)::[" << (socket == zyre_socket (_zyre)) << "]" << std::endl;
				*/
				
				if (socket == &(pipefds [0]))
				{
					std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::zpoller_wait()::socket == &(pipefds [0])" << std::endl;
					
					char buffer [6];
					
					// clear notifying bytes
					read (pipefds [0], buffer, 6);
					
					zpoller_destroy (&poller);
					poller = nullptr;
					
					break;
				}
				else if (zpoller_terminated (poller))
				{
					std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::zpoller_wait()::zpoller_terminated()" << std::endl;
					
					zpoller_destroy (&poller);
					poller = nullptr;
					
					break;
				}
				//if (!zpoller_terminated (poller)/* && !boost::this_thread::interruption_requested ()*/)
				//try
				//if (rc > 0)
				//{
					event = zyre_event_new (_zyre);
					std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::received an event" << std::endl;
				//}
				//if (event == nullptr)
				/*else// if (rc < 0)
				//catch (...)
				{
					//zyre_event_destroy (&event);
					//event = nullptr;
					
					zpoller_destroy (&poller);
					poller = nullptr;
				*/	/*
					std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::process was interrupted::" << errno << "::(" << std::strerror (errno) << ")" << std::endl;
					
					switch (errno)
					{
						// At least one of the members of the items array
						// refers to a socket whose associated ØMQ context was terminated.
						case ETERM:
							std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::process was interrupted::ETERM" << std::endl;
							break;
						
						// The provided items was not valid (NULL).
						case EFAULT:
							std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::process was interrupted::EFAULT" << std::endl;
							break;
						
						// The operation was interrupted by delivery of a signal
						// before any events were available.
						case EINTR:
							std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::process was interrupted::EINTR" << std::endl;
					}
					*/
				/*	std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::process was interrupted" << std::endl;
					
					break;
				}*/
				// Do not reiterate without trying to reach the boost interruption point that follows.
				/*else
				{
					continue;
				}*/
			//}
		/*}
		catch (...)
		{
			std::cerr << "zyre::receive()::interrupted" << std::endl;
		}*/
		
		 // Interrupted.
		//if (event == nullptr)
		//	break;
		
		/*
		if (event != nullptr)
		{
			std::cerr << "zyre::receive()ed an event::event_type==[" << zyre_event_type (event) << "]" << std::endl;
			//std::cerr << "zyre::receive()ed an event::uuid::local	==[" << id () << "]" << std::endl;
			std::cerr << "zyre::receive()ed an event::uuid::remote==[" << zyre_event_peer_uuid (event) << "]" << std::endl;
			//std::cerr << "zyre::receive()ed an event::ip addr==[" << zyre_event_peer_addr (event) << "]" << std::endl;
			std::cerr << "zyre::receive()ed an event::debug==[" << std::endl;
			zyre_event_print (event);
			std::cerr << "]==zyre::receive()ed an event::debug" << std::endl;
			
			if (zyre_event_peer_uuid (event) == _id && zyre_event_type (event) == "STOP")
			{
				zyre_event_destroy (&event);
				//zpoller_destroy (&poller);
				
				event = nullptr;
				//poller = nullptr;
				_id = "";
				
				break;
			}
		}
		*/
		
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::(event==nullptr)[" << (event == nullptr ? "true" : "false") << "]" << std::endl;
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::" << std::endl;
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::(_exoreception.empty())[" << (_exoreception.empty () ? "true" : "false") << "]" << std::endl;
		std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::(reception_is_set())[" << (reception_is_set () ? "true" : "false") << "]" << std::endl;
		
		if (event != nullptr)
		{
			//if (!_exoreception.empty ()/* && event != nullptr*/)
			if (reception_is_set ()/* && zyre_event_peer_uuid (event) != _id*/)
			{
				std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::delegating the event to the external handler" << std::endl;
				
				// Delegate to the external handler.
				_exoreception (event);
			}
			else
			{
				std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::not delegating the event to the external handler" << std::endl;
			}
			
			//if (verbose)
			//		zyre_event_print (event);
			
			/*
			if (streq (zyre_event_type (event), "ENTER")) {
					//  If new peer, say hello to it and wait for it to answer us
					zsys_info ("[%s] peer entered", zyre_event_peer_name (event));
					zyre_whispers (zyre, zyre_event_peer_uuid (event), "Hello");
			}
			else
			if (streq (zyre_event_type (event), "EXIT")) {
					zsys_info ("[%s] peer exited", zyre_event_peer_name (event));
			}
			else
			if (streq (zyre_event_type (event), "WHISPER")) {
					zsys_info ("[%s] received ping (WHISPER)", zyre_event_peer_name (event));
					zyre_shouts (zyre, "GLOBAL", "Hello");
			}
			else
			if (streq (zyre_event_type (event), "SHOUT")) {
					zsys_info ("[%s](%s) received ping (SHOUT)",
					           zyre_event_peer_name (event), zyre_event_group (event));
			}
			*/
			
			zyre_event_destroy (&event);
			event = nullptr;
		}
		
		try
		{
			boost::this_thread::interruption_point ();
		}
		//catch (...)
		catch (const boost::thread_interrupted &/* interruption*/)
		{
			std::cerr << "[" << boost::this_thread::get_id () << "] " << "zyre::receive()::caught boost::thread_interrupted" << std::endl;
			
			zpoller_destroy (&poller);
			poller = nullptr;
			break;
		}
	}
	//while (true);
	//while (event != nullptr);
	//while (inited ());
	//while (running ());
	//while (_running && inited ());
	//while (_running);
	
	//zpoller_destroy (&poller);
	//poller = nullptr;
}
