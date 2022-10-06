//
// C++ Implementation: bwdbregistry
//
// Description:
//
//
// Author: Bruno Moreira Guedes <bruno@beanweb.com.br>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define __BWDBREGISTRY_CPP
#include "bwdbregistry.h"
#include <bwresultset.h>
#include <bwdb.h>

#include <iostream>

using namespace std;

#define DUMP(VAR) (cerr << #VAR << ": " << VAR << endl)
#define DUMPI(VAR,INDEX) (cerr << #VAR << "[" << #INDEX << "=" << INDEX << "]=" << VAR[INDEX] << endl)


BWDbRegistry::BWDbRegistry(const BWDb &db,
                           const BWString &dbtable,
                           BWList &fields,
                           BWList &mreltables)
{
  bw_db = const_cast<BWDb*>(&db);
  reg_table = dbtable;
  SetTablePrefix("");
  SetIdField("");
  reg_fields = fields;
  reg_relations = mreltables;
  m_keepsync = false;
  m_dbchanged = false;
}


BWDbRegistry::~BWDbRegistry()
{
  //Save();
}




/*!
    \fn BWDbRegistry::saveRelation(const BWString &relation)
 */
void BWDbRegistry::saveRelation(const BWString &relation)
{
  DUMP(relation);
  BWList nrval, drval; //nrvla: lista para inserir; drval: lista para deletar
  register unsigned int i,j;
  bool still;
  BWResultSet rs;
  BWString reltab(reg_tablename + "_" + relation);
  BWString query, relid(relation + "_id"), regid(reg_table + "_id");
  BWKeyPair reldelins;

  reldelins[regid] = reg_id;

  if (!rel_new_values[relation].size()) {
              return;
  }

  if (!bw_db->AtTransaction())
  {
    throw BWError(DB_ERROR,
                  "Erro Salvando relacoes fora de transacao");
  }

  if (rel_new_values[relation].size() == 1 &&
      rel_new_values[relation][0] == "<%!EMPTY!%>") {

    if (m_keepsync) {
        delRegistry(reltab, regid, reg_id);
    }

    bw_db->Delete(reltab, reldelins);
    return;
  }

  for (i = 0; i < rel_curr_values[relation].size(); i++)
  {
    still = false;
    for (j = 0; j < rel_new_values[relation].size(); j++)
    {
      if (rel_curr_values[relation][i] ==
          rel_new_values[relation][j])
      {
        still = true;
      }
    }
    if (!still)
    {
      drval.push_back(rel_curr_values[relation][i]);
    }
  }
  for (i = 0; i < rel_new_values[relation].size(); i++)
  {
    still = false;
    for (j = 0; j < rel_curr_values[relation].size(); j++)
    {
      if (rel_new_values[relation][i] ==
          rel_curr_values[relation][j])
      {
        still = true;
      }
    }
    if (!still)
    {
      nrval.push_back(rel_new_values[relation][i]);
    }
  }

  for (i = 0; i < drval.size(); i++)
  {
    reldelins[relid]=drval[i];

    if (m_keepsync) {
      delRelations(reltab, reldelins);
    }

    bw_db->Delete(reltab, reldelins);
  }
  for (i = 0; i < nrval.size(); i++)
  {
    reldelins[relid] = nrval[i];

    if (m_keepsync) {
      reldelins["ultimaalteracao"] = get_date(BWMASK_DTNOW);
    }

    bw_db->Insert(reltab, reldelins);
  }
}


/*!
    \fn BWDbRegistry::Create()
 */
BWDbRegistry& BWDbRegistry::Create()
{
  bw_regnew = true;
  Clear();

  return *this;
}


/*!
    \fn BWDbRegistry::Delete()
 */
BWDbRegistry& BWDbRegistry::Delete()
{
  BWKeyPair delkpr, delrelkpr;
  register unsigned int i;
  
  if (bw_regnew) {
    throw BWError(DB_ERROR, "Erro ao deletar registro nao salvo");
  }

  try
  {
    bw_db->BeginTransaction();

    for (i = 0; i < reg_relations.size(); i++)
    {
      EmptyRelation(reg_relations[i]);
    }

    for (i = 0; i < reg_relations.size(); i++)
    {
      saveRelation(reg_relations[i]);
    }

    delkpr[reg_idfield_name] = reg_id;

    if (m_keepsync) {
      delRegistry(reg_tablename, reg_idfield_name, reg_id);
    }
    bw_db->Delete(reg_tablename, delkpr);
    bw_db->Commit();
  }
  catch (BWError &e)
  {
    e.TrackError(m_BWErrorPlace, "Erro ao deletar registro");
    try
    {
      if (bw_db->AtTransaction())
      {
        bw_db->Rollback();
      }
      throw e;
    }
    catch (BWError &ee)
    {
      ee.TrackError(m_BWErrorPlace,
                    "Erro ao tratar problemas em remocao de registro");
      throw BWFatalError(DB_ERROR,
                         "Erro fatal ao aplicar tratamento de erros");
    }
  }
  return *this;
}

/*!
    \fn BWDbRegistry::Insert()
 */
BWDbRegistry& BWDbRegistry::Insert()
{
  register unsigned int i;

  if (m_keepsync) {
      SetField("ultimaalteracao", get_date(BWMASK_DTNOW));
  }

  try
  {
    bw_db->BeginTransaction();
    bw_db->Insert(reg_tablename, reg_new_values);

    SetId(bw_db->GetLastId(reg_tablename, reg_idfield_name));

    for (i = 0; i < reg_relations.size(); i++)
    {
      saveRelation(reg_relations[i]);
    }

    bw_db->Commit();
  }
  catch (BWError &e)
  {
    e.TrackError(m_BWErrorPlace,
                 "Erro ao inserir dados. Tentando Rollback...");
    try
    {
      if (bw_db->AtTransaction())
      {
        bw_db->Rollback();
      }
      throw e;
    }
    catch (BWError &ee)
    {
      ee.TrackError(m_BWErrorPlace,
                    "Erro ao tratar problemas em insercao de registro");
      throw BWFatalError(DB_ERROR,
                         "Erro fatal no tratamento de erros");
    }
  }
  return *this;
}


/*!
    \fn BWDbRegistry::Load(BWKeyPair &kpr)
 */
BWDbRegistry& BWDbRegistry::Load(BWKeyPair &kpr)
{
  BWString q("SELECT ");
  BWResultSet rs;
  register unsigned int i;
  BWList fields,row;
  Clear();

  q += reg_fields.ToString() + " FROM " + reg_tablename + " WHERE " + kpr.ToSQLWhere();

  try
  {
    rs = bw_db->ExecuteQuery(q);
    if (!rs.GetNumRows()) {
                throw BWError(DB_ERROR,
                              "Erro ao carregar: registro inexistente");
    }
    row = rs.GetRow(1);
    fields = rs.GetFieldList();

    for (i = 0; i < fields.size(); i++)
    {
      reg_curr_values[fields[i]] = row[i];
    }

    reg_new_values = reg_curr_values;

    for (i = 0; i < reg_relations.size(); i++)
    {
      loadRelation(reg_relations[i]);
    }
    bw_regnew = false;
  }
  catch (BWError &e)
  {
    e.TrackError(m_BWErrorPlace, 
                 BWString("Erro carregando dados: SQL=") + q);
    throw e;
  }

  return *this;
}


/*!
    \fn BWDbRegistry::Load(int id)
 */
BWDbRegistry& BWDbRegistry::Load(int id)
{
  BWKeyPair k;
  reg_id = "";
  reg_id.AppendInt(id);

  k[reg_idfield_name] = reg_id;
  Load(k);

  return *this;
}


/*!
    \fn BWDbRegistry::Save()
 */
BWDbRegistry& BWDbRegistry::Save()
{
  if (bw_regnew)
  {
    Insert();
  }
  else
  {
    Update();
  }
  return *this;
}


/*!
    \fn BWDbRegistry::SaveAs(int id)
 */
BWDbRegistry& BWDbRegistry::SaveAs(int id)
{
  SetId(id);
  return Save();
}


/*!
    \fn BWDbRegistry::SaveAsNew()
 */
BWDbRegistry& BWDbRegistry::SaveAsNew()
{
  SetId(0);

  return Save();
}


/*!
    \fn BWDbRegistry::Set(BWKeyPair &fields)
 */
BWDbRegistry& BWDbRegistry::Set(BWKeyPair &fields)
{
  BWKeyPair::iterator i = fields.begin();

  for (; i != fields.end(); i++)
  {
    SetField(i->first, i->second);
  }

  return *this;
}

/*!
    \fn BWDbRegistry::SetDb(const BWDb &db)
 */
BWDbRegistry& BWDbRegistry::SetDb(const BWDb &db) {
  bw_db = const_cast<BWDb*>(&db);
  m_dbchanged = true;
  return *this;
}


/*!
    \fn BWDbRegistry::SetField(const BWString &field, const BWString &value)
 */
BWDbRegistry& BWDbRegistry::SetField(const BWString &field, const BWString &v)
{
  register unsigned int i;
  bool found = false;
  BWString value = v;

  value.Escape("\\");

//   cerr << field << "=" << value << endl;
  for (i = 0; i < reg_fields.size(); i++)
  {
    if (reg_fields[i] == field)
    {
      found = true;
      break;
    }
    else {
//             cerr << field << " nao eh " << reg_fields[i] << "!" << endl;
    }
  }

  if (found)
  {
    reg_new_values[field] = value;
  }
  else {
//               cerr << "Campo nao usado: " << field << "=" << value << endl;
  }

  return *this;
}


/*!
    \fn BWDbRegistry::SetRelation(const BWString &field, BWList &value)
 */
BWDbRegistry& BWDbRegistry::SetRelation(const BWString &field, BWList &value)
{
  register unsigned int i;

  for (i = 0; i < value.size(); i++)
  {
    SetRelation(field, value[i]);
  }

  return *this;
}


/*!
    \fn BWDbRegistry::SetRelation(const BWString &field, const BWString &newvalue)
 */
BWDbRegistry& BWDbRegistry::SetRelation(const BWString &field, const BWString &newvalue)
{
  register unsigned int i;
  bool found = false;

  for (i = 0; i < reg_relations.size(); i++)
  {
    if (reg_relations[i] == field)
    {
      found = true;
      break;
    }
  }

  if (found)
  {
    rel_new_values[field].push_back(newvalue);
  }

  return *this;
}


/*!
    \fn BWDbRegistry::Update()
 */
BWDbRegistry& BWDbRegistry::Update()
{
  BWKeyPair id;
  register unsigned int i;

  if (m_keepsync) {
    SetField("ultimaalteracao", get_date(BWMASK_DTNOW));
  }

  id[reg_idfield_name] = reg_id;
  try
  {
    bw_db->BeginTransaction();

    if (reg_new_values != reg_curr_values) {
            if (!bw_db->Update(reg_tablename, reg_new_values, id))
            {
                        throw BWError(DB_ERROR, "Erro em Update() do banco");
            }
    }

    for (i = 0; i < reg_relations.size(); i++)
    {
      saveRelation(reg_relations[i]);
    }

    bw_db->Commit();
  }
  catch (BWError &e)
  {
    e.TrackError(m_BWErrorPlace,
                 "Erro ao atualizar dados. Tentando Rollback...");
    try
    {
      if (bw_db->AtTransaction())
      {
        bw_db->Rollback();
      }
      throw e;
    }
    catch (BWError &ee)
    {
      ee.TrackError(m_BWErrorPlace,
                    "Erro ao tratar problemas em atualizacao de registro");
      throw BWFatalError(DB_ERROR,
                         "Erro fatal no tratamento de erros");
    }
  }

  Load(reg_id.GetInt());

  return *this;
}


/*!
    \fn BWDbRegistry::GetTable() const
 */
BWString BWDbRegistry::GetTable() const
{
  return reg_tablename;
}


/*!
    \fn BWDbRegistry::GetField(const BWString &field) const
 */
BWString BWDbRegistry::GetField(const BWString &field)
{
  return reg_curr_values[field];
}


/*!
    \fn BWDbRegistry::operator[](const BWString &field) const
 */
BWString BWDbRegistry::operator[](const BWString &field)
{
  return GetField(field);
}


/*!
    \fn BWDbRegistry::GetId() const
 */
int BWDbRegistry::GetId() const
{
  return reg_id.GetInt();
}


/*!
    \fn BWDbRegistry::GetRelations(const BWString &rel)
 */
BWString BWDbRegistry::GetRelations(const BWString &rel)
{
  return rel_curr_values[rel].ToString();
}


/*!
    \fn BWDbRegistry::AddField(const BWString &field)
 */
BWDbRegistry& BWDbRegistry::AddField(const BWString &field)
{
  reg_fields.push_back(field);
  return *this;
}


/*!
    \fn BWDbRegistry::AddRelation(const BWString &table)
 */
BWDbRegistry& BWDbRegistry::AddRelation(const BWString &table)
{
  reg_relations.push_back(table);
  return *this;
}


/*!
    \fn BWDbRegistry::SetIdField(const BWString &id)
 */
BWDbRegistry& BWDbRegistry::SetIdField(const BWString &id)
{
  register unsigned int i;
  bool found = false;

  if (id == "") {
            reg_idfield_name = "id";
  }
  else {
              reg_idfield_name = id;
  }

  for (i = 0; i < reg_fields.size(); i++)
  {
    if (reg_fields[i] == reg_idfield_name)
    {
      found = true;
    }
  }
  if (!found)
  {
    reg_fields.push_back(reg_idfield_name);
  }

  return *this;
}


/*!
    \fn BWDbRegistry::SetTablePrefix(const BWString &p)
 */
BWDbRegistry& BWDbRegistry::SetTablePrefix(const BWString &p)
{
  reg_tabprefix = p;
  if (p == "")
  {
    reg_tablename = reg_table;
  }
  else
  {
    reg_tablename = p + "_" + reg_table;
  }

  return *this;
}


/*!
    \fn BWDbRegistry::loadRelation(const BWString &rel)
 */
void BWDbRegistry::loadRelation(const BWString &rel)
{
  BWResultSet rs;
  BWString q("SELECT ");

  q += rel + "_id FROM " + reg_tablename + "_" + rel +
       " WHERE " + reg_table + "_" + reg_idfield_name + "='" + reg_id + "'";
  try
  {
    rs = bw_db->ExecuteQuery(q);
  }
  catch (BWError &e)
  {
    e.TrackError(m_BWErrorPlace,
                 BWString("Erro carregando relacao ") +
                 rel + "(tablename=" + reg_tablename + ")");
    throw e;
  }

  rel_curr_values[rel] = rs.GetColumn(rel + "_id");
}


/*!
    \fn BWDbRegistry::SetId(int id)
 */
BWDbRegistry& BWDbRegistry::SetId(int id)
{
  reg_id = "";

  if (id > 0)
  {
    reg_id.AppendInt(id);
  }
  else
  {
    bw_regnew = true;
  }

  return *this;
}


/*!
    \fn BWDbRegistry::Clear()
 */
BWDbRegistry& BWDbRegistry::Clear()
{
            reg_curr_values.clear();
            reg_new_values.clear();
            rel_new_values.clear();
            rel_curr_values.clear();

            return *this;
}


/*!
    \fn BWDbRegistry::EmptyRelation(const BWString &relation)
 */
BWDbRegistry& BWDbRegistry::EmptyRelation(const BWString &relation)
{
    rel_new_values[relation].clear();
    rel_new_values[relation].push_back("<%!EMPTY!%>");
    
    return *this;
}


/*!
    \fn BWDbRegistry::GetRelationOptions(const BWString &rel, const BWString &idfield, const BWString &descrfield, const BWString &reltab)
 */
BWKeyPair BWDbRegistry::GetRelationOptions(const BWString &rel, const BWString &idfield, const BWString &descrfield, const BWString &reltab)
{
    ///@todo Organizar GetRelationOptions e GetFieldOptions.
    ///@todo A ideia inicial do metodo era retornar da base de dados em 1 query
    ///@todo unica as opcoes de relacao. Porem, a evolucao da classe impediu
    ///@todo que isso ocorresse de maneira que o codigo fosse reaproveitavel. 
    ///@todo Muitos problemas nao-previstos apareceram, e o pequeno metodo
    ///@todo reaproveitaveis teve que ficar enorme. REORGANIZAR OS CODIGOS!!!
    BWKeyPair ret;
    BWResultSet rs;
    BWString query("SELECT ");
    BWString key, val;

//     cerr << "Entrando em GetRelationOptionIds()" << endl;
//     DUMP(rel);
//     DUMP(idfield);
//     DUMP(descrfield);
//     DUMP(reltab);

    if (descrfield == "" || idfield == "") {
            val = idfield + descrfield;
            query += val;
            if (val == "") {
                        key = "Erro Fatal ao obter relacao ";
                        key += rel + " com campo de id e de descricao vazios";
                        throw BWFatalError(CGI_ERROR, key);
            }
            if (val != "id") {
                key = "id";
                query += ",id";
            }
            else {
                key = val;
            }
    }
    else {
            query += idfield + "," + descrfield;
            key = idfield;
            val = descrfield;
    }
    if (reltab == "") {
            query  += " FROM " + reg_tablename + "_" + rel;
    }
    else {
                 query += " FROM ";
                 if (reg_tabprefix == "" || reg_tabprefix == "(null)") {
                  query += reltab;
                 }
                 else {
                  query += reg_tabprefix + "_" + reltab;
                 }
    }
    query += " ORDER BY id";

    try {
                rs = bw_db->ExecuteQuery(query);
                while (rs.Next()) {
                            BWString descrs_id, descrs_descr;
//                             descrs_id = rs.GetString(key);
//                             descrs_descr = rs.GetString(val);
//                             DUMP(descrs_id);
//                             DUMP(descrs_descr);
                            ret[rs.GetString(key)] = rs.GetString(val);
                }
    }
    catch(BWError &e) {
                e.TrackError(m_BWErrorPlace,
                             BWString("ALERTA: Erro ao obter opcoes da relacao ") +
                             rel);
                if (e.IsFatal()) {
                            throw e;
                }
                else {
                            ret.clear();
                            return ret;
                }
    }
//     cerr << "Saindo em GetRelationOptionIds()" << endl;
    
    return ret;
}


/*!
    \fn BWDbRegistry::GetRelationOptionIds(const BWString &relation)
 */
BWList BWDbRegistry::GetRelationOptionIds(const BWString &relation, const BWString &idfield)
{
    BWList ret;
//     DUMP(relation);
//     DUMP(idfield);
    BWKeyPair kpr = GetRelationOptions(relation, idfield, "", relation);
    BWKeyPair::iterator itr = kpr.begin();
    
    while (itr != kpr.end()) {
//                 DUMP(itr->first);
                if (idfield == "id") {
                        ret.push_back(itr->first);
                }
                else {
                        ret.push_back(itr->second);
                }
                itr++;
    }
    
    for(register unsigned int i = 0; i < ret.size(); i++) {
        DUMP(i);
        DUMPI(ret, i);
    }
    
    return ret;
}


/*!
    \fn BWDbRegistry::GetRelationOptionDescrs(const BWString &rel)
 */
BWList BWDbRegistry::GetRelationOptionDescrs(const BWString &rel, const BWString &descrfield)
{
            return GetRelationOptionIds(rel, descrfield);
}

/*!
    \fn BWDbRegistry::GetFieldOptions(const BWString &rel)
 */
BWKeyPair BWDbRegistry::GetFieldOptions(const BWString &field, const BWString &optidfield, const BWString &optdescrfield)
{
            BWString nfield = field;
//             cerr << "nfield=" << nfield << endl;
            nfield.substr(0, 
                          nfield.size() - (reg_idfield_name.size() + 1)); 
            //retira "_" + reg_idfield_name
//             cerr << "nfield=" << nfield << endl;

            return GetRelationOptions("", optidfield, optdescrfield, nfield);
}


/*!
    \fn BWDbRegistry::GetFieldOptionIds(const BWString &relation)
 */
BWList BWDbRegistry::GetFieldOptionIds(const BWString &relation, const BWString &idfield)
{
            BWList ret;
            BWKeyPair kpr = GetFieldOptions(relation, idfield, "");
            BWKeyPair::iterator itr = kpr.begin();
    
            while (itr != kpr.end()) {
//                         cerr << ":" << itr->first << ":" << endl;
                        if (idfield == "id") {
                                    ret.push_back(itr->first);
                        }
                        else {
                                    ret.push_back(itr->second);
                        }
                        itr++;
            }
    
            return ret;
}


/*!
    \fn BWDbRegistry::GetFieldOptionDescrs(const BWString &rel)
 */
BWList BWDbRegistry::GetFieldOptionDescrs(const BWString &rel, const BWString &descrfield)
{
            return GetFieldOptionIds(rel, descrfield);
}

void BWDbRegistry::SetKeepSync() {
    m_keepsync = true;

    AddField("ultimaalteracao");
}

void BWDbRegistry::delRegistry(const BWString &table, const BWString &idfield,
                               const BWString &idvalue) {
    BWKeyPair record;

    record["tabela"] = table;
    record["ultimaalteracao"] = get_date(BWMASK_DTNOW);

    bw_db->Insert("bw_ksremovido", record);

    record.clear();
    record["ksremovido_id"] = BWString("").AppendInt(
                                bw_db->GetLastId("bw_ksremovido"));
    record["chave"] = idfield;
    record["valor"] = idvalue;

    bw_db->Insert("bw_ksremovidoassoc", record);
}

void BWDbRegistry::delRelations(const BWString &table, BWKeyPair data) {
    BWKeyPair record;
    BWKeyPair::iterator itr;
    BWString id;

    record["tabela"] = table;
    record["ultimaalteracao"] = get_date(BWMASK_DTNOW);

    bw_db->Insert("bw_ksremovido", record);

    id.AppendInt(bw_db->GetLastId("bw_ksremovido"));

    for (itr = data.begin(); itr != data.end(); itr++) {
        record.clear();
        record["ksremovido_id"] = id;
        record["chave"] = itr->first;
        record["valor"] = itr->second;

        bw_db->Insert("bw_ksremovidoassoc", record);
    }
}

bool BWDbRegistry::DbChanged() {
    return m_dbchanged;
}
