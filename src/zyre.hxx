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
		virtual const bool running (void) const;
		virtual const bool start (void);
		
		virtual const bool leave (const std::string &);
		virtual const bool joined (const std::string &) const;
		virtual const bool join (const std::string &);
		
		virtual const std::string id (void) const;
		
		// Own groups.
		virtual const std::map <const unsigned int, const std::string> groups_own (void) const;
		// Groups known via peers.
		virtual const std::map <const unsigned int, const std::string> groups_peers (void) const;
		
		// All peers.
		virtual const std::map <const unsigned int, const std::string> peers (void) const;
		// Peers belonging to a group.
		virtual const std::map <const unsigned int, const std::string> peers (const std::string & group) const;
		
		// Transmit a message to one peer.
		virtual const bool unicast (const zmq::msg &/* multipart message*/, const std::string &/* peer id*/) const;
		//const bool unicast (const zmsg_t */* multipart message*/, const std::string &/* peer id*/) const;
		
		// Get a random peer (ID).
		virtual const std::string any (void) const;
		
		// Get a random peer (ID) from this specified group.
		virtual const std::string any (const std::string &/* group*/) const;
		
		// Transmit a message to any one peer amongst all peers.
		virtual const bool anycast (const zmq::msg &/* multipart message*/, std::string &/* peer_id*/) const;
		
		// Transmit a message to any one peer belonging to a group.
		virtual const bool anycast (const zmq::msg &/* multipart message*/, std::string &/* peer_id*/, const std::string &/* group*/) const;
		
		// Transmit a message to all peers belonging to a group.
		virtual const bool multicast (const zmq::msg &/* multipart message*/, const std::string &/* group*/) const;
		//const bool multicast (const zmsg_t */* multipart message*/, const std::string &/* group*/) const;
		
		// Transmit a message to all peers.
		virtual const bool broadcast (const zmq::msg &/* multipart message*/) const;
		
		
		virtual const zyre_t * operator * (void) const;
		
		// units count cluster_size peers_count
		virtual const unsigned int peers_size (void) const;
		virtual const unsigned int peers_size (const std::string &/* group*/) const;
		// groups
		
		// These three functions (internally) refer to "exoreception".
		virtual const bool reception_unset (void);
		virtual const bool reception_is_set (void) const;
		virtual const bool reception_set
		(
			const boost::function
			<
				void/* manage*/
				(
					const zyre_event_t */* zyre_event*/
				)
			>
			&/* manager*/
		);
	protected:
		zyre_t * _zyre;
		
		bool _running;
		
		// For running "void receive (void)".
		boost::thread * _reception;
		
		// External (for the object) event handler.
		boost::function <void/* manage*/ (const zyre_event_t */* zyre_event*/)> _exoreception;
		
		// Manager of incoming messages.
		virtual void receive (void);
		
		// Randomness
		//boost::random::random_device randev;
		//std::mt19937 gen(rd());
		//boost::random::mt19937 gentor {randev ()};
};
