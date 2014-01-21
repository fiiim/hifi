//
//  ParticlesScriptingInterface.cpp
//  hifi
//
//  Created by Brad Hefta-Gaub on 12/6/13
//  Copyright (c) 2013 HighFidelity, Inc. All rights reserved.
//

#include "ParticlesScriptingInterface.h"



void ParticlesScriptingInterface::queueParticleMessage(PACKET_TYPE packetType,
        ParticleID particleID, const ParticleProperties& properties) {
    getParticlePacketSender()->queueParticleEditMessage(packetType, particleID, properties);
}

ParticleID ParticlesScriptingInterface::addParticle(const ParticleProperties& properties) {

    // The application will keep track of creatorTokenID
    uint32_t creatorTokenID = Particle::getNextCreatorTokenID();

    ParticleID id(NEW_PARTICLE, creatorTokenID, false );

    // queue the packet
    queueParticleMessage(PACKET_TYPE_PARTICLE_ADD_OR_EDIT, id, properties);

    return id;
}

void ParticlesScriptingInterface::editParticle(ParticleID particleID, const ParticleProperties& properties) {
    uint32_t actualID = particleID.id;
    if (!particleID.isKnownID) {
        actualID = Particle::getIDfromCreatorTokenID(particleID.creatorTokenID);

        // hmmm... we kind of want to bail if someone attempts to edit an unknown
        if (actualID == UNKNOWN_PARTICLE_ID) {
            return; // bailing early
        }
    }

    particleID.id = actualID;
    particleID.isKnownID = true;

    queueParticleMessage(PACKET_TYPE_PARTICLE_ADD_OR_EDIT, particleID, properties);
}


// TODO: This deleteParticle() method uses the PACKET_TYPE_PARTICLE_ADD_OR_EDIT message to send
// a changed particle with a shouldDie() property set to true. This works and is currently the only
// way to tell the particle server to delete a particle. But we should change this to use the PACKET_TYPE_PARTICLE_ERASE
// message which takes a list of particle id's to delete.
void ParticlesScriptingInterface::deleteParticle(ParticleID particleID) {

    // setup properties to kill the particle
    ParticleProperties properties;
    properties.setShouldDie(true);

    uint32_t actualID = particleID.id;
    if (!particleID.isKnownID) {
        actualID = Particle::getIDfromCreatorTokenID(particleID.creatorTokenID);

        // hmmm... we kind of want to bail if someone attempts to edit an unknown
        if (actualID == UNKNOWN_PARTICLE_ID) {
            //qDebug() << "ParticlesScriptingInterface::deleteParticle(), bailing - unknown particle...";
            return; // bailing early
        }
    }

    particleID.id = actualID;
    particleID.isKnownID = true;

    //qDebug() << "ParticlesScriptingInterface::deleteParticle(), queueParticleMessage......";
    queueParticleMessage(PACKET_TYPE_PARTICLE_ADD_OR_EDIT, particleID, properties);
}
