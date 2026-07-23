#pragma once

// caching: storing loaded textures, sounds, fonts etc. in hash maps or dicts using file paths as keys to avoid duplicate loading
//
// reference system (handles vs raw pointers)
//
// thread awareness: opengl commands must happen on the main thread, IO and disk reads can hapen or worker threads
// 
// unloading: centralized unloadtexture and unload sound calls when changing screens or closing game
//
// packaging: use rres to pack multiple assets into a single file for fast and efficient loading
//
// use shared and weak pointers to automatically free assets when no objects use them anymore.
