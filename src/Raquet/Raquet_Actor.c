#include "Raquet.h"

Raquet_Actor * Raquet_AllocateActor(void) {
    return ( Raquet_Actor * ) malloc(sizeof(Raquet_Actor));
}

void Raquet_CreateActor(Raquet_Actor * act, Raquet_CHR chr) {
    act -> position.x = 0;
    act -> position.y = 0;
    act -> origin.x = 0;
    act -> origin.y = 0;
    act -> angle = 0;
    act -> flip = SDL_FLIP_NONE;
    Raquet_Point size = { chr.width, chr.height };
    act -> chr = chr;
    act -> width = size.x;
    act -> height = size.y;
    act -> bbox.x1 = 0;
    act -> bbox.y1 = 0;
    act -> bbox.x2 = size.x;
    act -> bbox.y2 = size.y;

}

void Raquet_DrawActor(Raquet_Actor * act) {
    Raquet_PlaceCHR_ext(act -> chr, act -> position.x - Camera.x, act -> position.y - Camera.y, act -> width, act -> height, act -> angle, act -> origin, act -> flip);
    #ifdef VISUALIZE_BBOX
        int* x = &act -> position.x;
        int* y = &act -> position.y;
        Raquet_DrawRectangle(*x - Camera.x + act -> bbox.x1, *y - Camera.y + act -> bbox.y1, act -> bbox.x2, act -> bbox.y2, 0xFF00FFFF, 255, 0);
    #endif
}

int Raquet_ActorColliding(int x, int y, Raquet_Actor * act1, Raquet_Actor * act2) {
    return (x - act1 -> origin.x + act1 -> bbox.x2 > act2 -> position.x - act2 -> origin.x + act2 -> bbox.x1) && (x - act1 -> origin.x + act1 -> bbox.x1 < act2 -> position.x - act2 -> origin.x + act2 -> bbox.x2) && (y - act1 -> origin.y + act1 -> bbox.y2 > act2 -> position.y - act2 -> origin.y + act2 -> bbox.y1) && (y - act1 -> origin.y + act1 -> bbox.y1 < act2 -> position.y - act2 -> origin.y + act2 -> bbox.y2);
}

void Raquet_FreeActor(Raquet_Actor* actor) {
    free(actor);
}
