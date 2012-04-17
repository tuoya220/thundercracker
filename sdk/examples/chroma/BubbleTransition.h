/* -*- mode: C; c-basic-offset: 4; intent-tabs-mode: nil -*-
 *
 * Handle bubble sprites
 * Copyright <c> 2012 Sifteo, Inc. All rights reserved.
 */

#ifndef _BUBBLETRANSITION_H
#define _BUBBLETRANSITION_H

#include <sifteo.h>
#include "game.h"

//take control and do transition
static void DoBubbleTransition()
{
    const unsigned int NUM_BUBBLES = 8;
    const float ANIM_LENGTH = 0.7f;
    const float BUBBLE_Y_VEL = 128.0f / ANIM_LENGTH * 2.0f;
    const float VEL_ADJUSTMENT_MIN = 0.8f;
    const float VEL_ADJUSTMENT_MAX = 1.2f;

    SystemTime startTime = SystemTime::now();
    SystemTime lastTime = startTime;

    Float2 BubblePos[NUM_CUBES][NUM_BUBBLES];

    //initialize bubble positions
    for( int i = 0; i < NUM_CUBES; i++ )
    {
        Game::Inst().m_cubes[i].ClearBG1();
        Game::Inst().m_cubes[i].FlushBG1();
        for( unsigned int j = 0; j < NUM_BUBBLES; j++ )
        {
            float yPos = Game::random.uniform( 128.0f, 160.0f );
            BubblePos[i][j] = vec<float>( Game::random.uniform( 0.0f, 16.0f ) + ( j * 16.0f ), yPos );
        }
    }

    while( SystemTime::now() - startTime < ANIM_LENGTH )
    {
        float delta = SystemTime::now() - lastTime;
        unsigned int frame = float( SystemTime::now() - startTime ) / ANIM_LENGTH * bubbles1.numFrames();

        if( frame >= bubbles1.numFrames() )
            frame = bubbles1.numFrames() - 1;

        for( int i = 0; i < NUM_CUBES; i++ )
        {
            VideoBuffer &vid = Game::Inst().m_cubes[i].GetVid();

            for( unsigned int j = 0; j < NUM_BUBBLES; j++ )
            {
                vid.sprites[j].setImage(bubbles1, frame);
                vid.sprites[j].move(BubblePos[i][j]);

                BubblePos[i][j].y += Game::random.uniform( -2.5f, 2.5f );
                BubblePos[i][j].y -= BUBBLE_Y_VEL * delta * Game::random.uniform( VEL_ADJUSTMENT_MIN, VEL_ADJUSTMENT_MAX );

                //fill in universal bg
                Int2 offset = { j*2, (int)( BubblePos[i][j].y / 8 ) + 1 };

                if( offset.y >= 0 && offset.y < 16 )
                    vid.bg0.image( offset, vec( 2, 16 - offset.y ), UI_BG, offset );
            }
        }

        System::paint();
        lastTime += delta;
    }

    for( int i = 0; i < NUM_CUBES; i++ )
    {
        VideoBuffer &vid = Game::Inst().m_cubes[i].GetVid();

        vid.bg0.image( vec( 0, 0 ), UI_BG );

        for( unsigned int j = 0; j < NUM_BUBBLES; j++ )
        {
            vid.sprites[j].hide();
        }
    }

    System::paint();
}



#endif //_BUBBLETRANSITION_H
