/**************************************************************************************/
/*                                                                                    */
/*  Visualization Library                                                             */
/*  http://visualizationlibrary.org                                                   */
/*                                                                                    */
/*  Copyright (c) 2005-2017, Michele Bosi                                             */
/*  All rights reserved.                                                              */
/*                                                                                    */
/*  Redistribution and use in source and binary forms, with or without modification,  */
/*  are permitted provided that the following conditions are met:                     */
/*                                                                                    */
/*  - Redistributions of source code must retain the above copyright notice, this     */
/*  list of conditions and the following disclaimer.                                  */
/*                                                                                    */
/*  - Redistributions in binary form must reproduce the above copyright notice, this  */
/*  list of conditions and the following disclaimer in the documentation and/or       */
/*  other materials provided with the distribution.                                   */
/*                                                                                    */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND   */
/*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED     */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE            */
/*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR  */
/*  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    */
/*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      */
/*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON    */
/*  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS     */
/*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                      */
/*                                                                                    */
/**************************************************************************************/

#include <vlX//ioVLX.hpp>
#include <vlCore/Time.hpp>

using namespace vl;

//-----------------------------------------------------------------------------
ref<ResourceDatabase> vlX::loadVLT(const String& path)
{
  vl::ref<vl::VirtualFile> file = vl::locateFile(path);
  return loadVLT(file.get());
}
//-----------------------------------------------------------------------------
ref<ResourceDatabase> vlX::loadVLT(vl::VirtualFile* file)
{
  vlX::VLXSerializer serializer;

  ref<Object> obj = serializer.loadVLT(file);

  if (serializer.error())
    Log::error( Say("vl::loadVLT : vlX::VLXSerializer reported: %s.\n") << serializer.errorString() );

  if (!obj)
    return NULL;

  ref<ResourceDatabase> res_db = obj->as<ResourceDatabase>();

  return res_db;
}
//-----------------------------------------------------------------------------
ref<ResourceDatabase> vlX::loadVLB(const String& path)
{
  vl::ref<vl::VirtualFile> file = vl::locateFile(path);
  return loadVLB(file.get());
}
//-----------------------------------------------------------------------------
ref<ResourceDatabase> vlX::loadVLB(vl::VirtualFile* file)
{
  vlX::VLXSerializer serializer;

  ref<Object> obj = serializer.loadVLB(file);

  if (serializer.error())
    Log::error( Say("vl::loadVLB : vlX::VLXSerializer reported: %s.\n") << serializer.errorString() );

  if (!obj)
    return NULL;

  ref<ResourceDatabase> res_db = obj->as<ResourceDatabase>();

  return res_db;
}
//-----------------------------------------------------------------------------
bool vlX::saveVLT(const String& path, const ResourceDatabase* res_db)
{
  ref<DiskFile> file = new DiskFile(path);
  return saveVLT(file.get(), res_db);
}
//-----------------------------------------------------------------------------
bool vlX::saveVLT(vl::VirtualFile* file, const ResourceDatabase* res_db)
{
  VL_CHECK(res_db);
  if (!res_db)
    return false;

  vlX::VLXSerializer serializer;
  serializer.saveVLT( file, res_db );

  if (serializer.error())
    Log::error( Say("vlX::saveVLT : vlX::VLXSerializer reported: %s.\n") << serializer.errorString() );

  return serializer.error() == vlX::VLXSerializer::NoError;
}
//-----------------------------------------------------------------------------
bool vlX::saveVLB(const String& path, const ResourceDatabase* res_db)
{
  ref<DiskFile> file = new DiskFile(path);
  return saveVLB(file.get(), res_db);
}
//-----------------------------------------------------------------------------
bool vlX::saveVLB(vl::VirtualFile* file, const ResourceDatabase* res_db)
{
  VL_CHECK(res_db);
  if (!res_db)
    return false;

  vlX::VLXSerializer serializer;
  serializer.saveVLB( file, res_db );

  if (serializer.error())
    Log::error( Say("vlX::saveVLB : vlX::VLXSerializer reported: %s.\n") << serializer.errorString() );

  return serializer.error() == vlX::VLXSerializer::NoError;
}
//-----------------------------------------------------------------------------
bool vlX::isVLT(const vl::String& path)
{
  vl::ref<vl::VirtualFile> file = vl::locateFile(path);
  return isVLT(file.get());
}
//-----------------------------------------------------------------------------
bool vlX::isVLT(vl::VirtualFile* file)
{
  if (!file)
    return false;
  char vlx[12] = { 0 };
  memset(vlx, 0, sizeof(vlx));
  file->close();
  file->open(OM_ReadOnly);
  file->read(vlx, sizeof(vlx));
  file->close();
  return memcmp(vlx, "VLX version=", sizeof(vlx)) == 0;
}
//-----------------------------------------------------------------------------
bool vlX::isVLB(const vl::String& path)
{
  vl::ref<vl::VirtualFile> file = vl::locateFile(path);
  return isVLT(file.get());
}
//-----------------------------------------------------------------------------
bool vlX::isVLB(vl::VirtualFile* file)
{
  if (!file)
    return false;
  unsigned char vlx_identifier[] = { 0xAB, 'V', 'L', 'X', 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };
  unsigned char vlx[sizeof(vlx_identifier)];
  memset(vlx, 0, sizeof(vlx));
  file->close();
  file->open(OM_ReadOnly);
  file->read(vlx, sizeof(vlx));
  file->close();
  return memcmp(vlx, vlx_identifier, sizeof(vlx_identifier)) == 0;
}
//-----------------------------------------------------------------------------
