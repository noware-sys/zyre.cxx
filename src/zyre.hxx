#pragma once

//#include "cast.hxx"

#include <zmq.hpp>
#include <zhelpers.hpp>
#include <zyre.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>
//#include <boost/function_equal.hpp>
#include <boost/thread.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <string>
#include <map>

//#include "cluster.hxx"
//#include "../tree.hxx"
//#include "../var.hxx"
#include <zmq/msg.hxx>

class zyre
{
	public:
		zyre (void);
		virtual ~zyre (void);
		
		virtual const bool fin (void);
		virtual const bool inited (void) const;
		virtual const bool init (void);
		
		virtual const bool stop (void);
		// running
		// active activated acting
		virtual const bool running (void) const;
		virtual const bool start (void);
		
		virtual const bool leave (const std::string &);
		virtual const bool joined (const std::string &) const;
		virtual const bool join (const std::string &);
		
		virtual const std::string id (void) const;
		//virtual const std::string name (void) const;	// Get name
		//virtual const bool name (const std::string & new_name);	// Set name
		//virtual const bool verbosity (const unsigned int & new_verbosity);	// Set verbosity
		//virtual const bool interface (const std::string & new_interface)	// Set interface
		
		// Own groups.
		virtual const std::map <const unsigned int, const std::string> grp_own (void) const;
		// Groups known via peers.
		virtual const std::map <const unsigned int, const std::string> grp_peer (void) const;
		
		// All peers.
		virtual const std::map <const unsigned int, const std::string> peer (void) const;
		// Peers belonging to a group.
		virtual const std::map <const unsigned int, const std::string> peer (const std::string &/* grp*/) const;
		
		// Transmit a message to own self.
		virtual const bool loccast (const zmq::msg &/* multipart message*/) const;
		
		// Transmit a message to one peer.
		virtual const bool unicast (const zmq::msg &/* multipart message*/, const std::string &/* peer id*/) const;
		//const bool unicast (const zmsg_t */* multipart message*/, const std::string &/* peer id*/) const;
		
		// Get a random peer (ID).
		virtual const std::string any (void) const;
		
		// Get a random peer (ID) from this specified group.
		virtual const std::string any (const std::string &/* group*/) const;
		
		// Transmit a message to any one peer amongst all peers.
		virtual const bool anycast (const zmq::msg &/* multipart message*/, std::string &/* transmitted-to peer_id*/) const;
		
		// Transmit a message to any one peer belonging to a group.
		virtual const bool anycast (const zmq::msg &/* multipart message*/, std::string &/* transmitted-to peer_id*/, const std::string &/* group*/) const;
		
		// Transmit a message to all peers belonging to a group.
		virtual const bool multicast (const zmq::msg &/* multipart message*/, const std::string &/* group*/) const;
		//const bool multicast (const zmsg_t */* multipart message*/, const std::string &/* group*/) const;
		
		// Transmit a message to all peers.
		virtual const bool broadcast (const zmq::msg &/* multipart message*/) const;
		
		
		virtual zyre_t * operator * (void);
		
		// units count cluster_size peers_count
		virtual unsigned int const peer_size (void) const;
		virtual unsigned int const peer_size (const std::string &/* group*/) const;
		// groups
		
		// These three functions (internally) refer to "exoreception".
		virtual const bool rx_unset (void);
		virtual const bool rx_is_set (void) const;
		virtual const bool rx_set
		(
			const boost::function
			<
				void/* manage*/
				(
					zyre_event_t * const/* zyre_event*/
				)
			>
			&/* manager*/
		);
		
		bool sync;
		std::map <boost::thread::id, boost::thread *> call_async;
	protected:
		zyre_t * _zyre;
		//zpoller_t * poller;
		int s_fd;
		
		bool _running;
		
		// For running "void receive (void)".
		boost::thread * _rx;
		
		// External (for the object) event handler.
		boost::function <void/* manage*/ (zyre_event_t * const/* zyre_event*/)> _exorx;
		virtual void _exorx_call (zyre_event_t * const/* zyre_event*/);
		
		// Manager of incoming messages.
		virtual void rx (void);
		
		void s_signal_handler (int/* signal_value*/);
		void s_catch_signal (int/* fd*/);
		
		//std::string _id;
		
		// Randomness
		//boost::random::random_device randev;
		//std::mt19937 gen(rd());
		//boost::random::mt19937 gentor {randev ()};
};
