#ifndef SRC_MAIN_H
#define	SRC_MAIN_H

#include <commands/packet.h>
#include <controller/state.h>
#include <platforms/platform.h>
#include <shared/versions.h>

void main_init(volatile controller_state *state);
void main_loop(volatile controller_state *state);

#endif	/* SRC_MAIN_H */
