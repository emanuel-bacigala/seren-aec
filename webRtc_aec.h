/*
 * Copyright (C) 2024 Matus Jurecka
 *
 * This file is part of Seren.
 *
 * Seren is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Seren is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdint.h>

int webRTC_aecInit(unsigned rate, int16_t** farData, int16_t** nearData);
int webRTC_aecProcessFarEnd();
int webRTC_aecProcessNearEnd(int delay);
int webRTC_aecDeinit(void);
