//
// C++ Implementation: bwdbrelation
//
// Description:
//
//
// Author: Samuel da Silva Feitosa <samuel@beanweb.com.br>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bwdbrelation.h"

BWDbRelation::BWDbRelation(BWDb *db, const BWString &id,
                           const BWString &from, const BWString &to,
                           bool invert) {
    m_db = db;
    m_id = id;
    m_from = from;
    m_to = to;
    m_invert = invert;
    m_keepsync = false;
}


BWDbRelation::~BWDbRelation() {
}

void BWDbRelation::SetFields(const BWString &l, const BWString &r) {
    m_lfield = l;
    m_rfield = r;
}

void BWDbRelation::SetTables(const BWString &l, const BWString &r) {
    m_ltable = l;
    m_rtable = r;
}

void BWDbRelation::SetKeepSync() {
    m_keepsync = true;
}

void BWDbRelation::Save() {
    BWList ins;
    BWList del;

    if (m_new_values[m_rfield].size() <= 0) {
        // Nothing to do
        return;
    }

    if (!m_db->AtTransaction()) {
        throw BWError(DB_ERROR, "Erro: Salvando relacoes fora da transacao");
    }

    // Este teste indica que a lista de itens foi zerada, e portanto
    // devemos deletar todos os valores correspondentes a este ID
    if (m_new_values[m_rfield].size() == 1 &&
        m_new_values[m_rfield][0] == "<%!EMPTY!%>") {

        emptyItens();

        return;
    }

    del = getDeleteList();
    ins = getInsertList();

    deleteItens(del);
    insertItens(ins);
}

void BWDbRelation::emptyItens() {
    BWKeyPair values;

    values[m_lfield] = m_id;

    if (m_keepsync) {
        // Informar ao keepsync que esta sendo removido
    }

    m_db->Delete(m_table, values);
}

void BWDbRelation::deleteItens(BWList &del) {
    BWKeyPair values;

    values[m_lfield] = m_id;

    for (unsigned int i = 0; i < del.size(); i++) {
        values[m_rfield] = del[i];

        if (m_keepsync) {
            // Informar ao keepsync que esta sendo removido
            //delRelations(reltab, reldelins);
        }

        m_db->Delete(m_table, values);
    }
}

void BWDbRelation::insertItens(BWList &ins) {
    BWKeyPair values;

    values[m_lfield] = m_id;

    for (unsigned int i = 0; i < ins.size(); i++) {
        values[m_rfield] = ins[i];

        if (m_keepsync) {
            values["ultimaalteracao"] = get_date(BWMASK_DTNOW);
        }

        m_db->Insert(m_table, values);
    }       
}

BWList BWDbRelation::getDeleteList() {
    BWList list;
    bool still;

    for (unsigned int i = 0; i < m_cur_values[m_rfield].size(); i++) {
        still = false;
        for (unsigned int j = 0; j < m_new_values[m_rfield].size(); j++) {
            if (m_cur_values[m_rfield][i] == m_new_values[m_rfield][j]) {
                still = true;
            }
        }
        if (!still) {
            list.push_back(m_cur_values[m_rfield][i]);
        }
    }

    return list;
}

BWList BWDbRelation::getInsertList() {
    BWList list;
    bool still;
  
    for (unsigned int i = 0; i < m_new_values[m_rfield].size(); i++) {
        still = false;
        for (unsigned int j = 0; j < m_cur_values[m_rfield].size(); j++) {
            if (m_new_values[m_rfield][i] == m_cur_values[m_rfield][j]) {
                still = true;
            }
        }
        if (!still) {
            list.push_back(m_new_values[m_rfield][i]);
        }
    }

    return list;
}

/*
void BWDbRelation::Save() {
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

  // itens a deletar
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
  // itens a inserir
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

  // delete

  for (i = 0; i < drval.size(); i++)
  {
    reldelins[relid]=drval[i];

    if (m_keepsync) {
      delRelations(reltab, reldelins);
    }

    bw_db->Delete(reltab, reldelins);
  }

  // insert

  for (i = 0; i < nrval.size(); i++)
  {
    reldelins[relid] = nrval[i];

    if (m_keepsync) {
      reldelins["ultimaalteracao"] = get_date(BWMASK_DTNOW);
    }

    bw_db->Insert(reltab, reldelins);
  }
}
*/

void BWDbRelation::Add(BWList &value) {
    unsigned int i;

    for (i = 0; i < value.size(); i++) {
        AddValue(value[i]);
    }
}


void BWDbRelation::AddValue(const BWString &value) {
    // Ver se eh necessario verificar se item ja esta na lista...
    m_new_values[m_rfield].push_back(value);
}


BWString BWDbRelation::Get(bool left) {
    if (left) {
        return m_cur_values[m_lfield].ToString();
    }

    return m_cur_values[m_rfield].ToString();
}

// Por enquanto pegando somente o da direita
BWKeyList BWDbRelation::GetInfo(BWList &fields) {
    BWKeyList klist;
    BWResultSet rset;
    BWString query;

    if (m_order == "") {
        m_order = fields[0];
    }

    query  = "SELECT ";
    query += fields.ToString();
    query += " FROM " + m_rtable;
    query += " ORDER BY " + m_order;

    rset = m_db->ExecuteQuery(query);

    for (unsigned int i = 0; i < fields.size(); i++) {
        klist[fields[i]] = rset.GetColumn(fields[i]);
    }

    return klist;
}


void BWDbRelation::Load(const BWString &lid, const BWString &rid) {
    LoadL(lid);
    LoadR(rid);
}

void BWDbRelation::LoadL(const BWString &id) {
    BWString query;
    BWResultSet rset;

    query  = "SELECT ";
    query += m_lfield;
    query += " FROM " + m_table;

    if (id.length() > 0) {
        query += " WHERE " + m_rfield;
        query += " = '" + id + "'";
    }

    rset = m_db->ExecuteQuery(query);

    m_cur_values[m_lfield] = rset.GetColumn(m_lfield);
}

void BWDbRelation::LoadR(const BWString &id) {
    BWString query;
    BWResultSet rset;

    query  = "SELECT ";
    query += m_rfield;
    query += " FROM " + m_table;

    if (id.length() > 0) {
        query += " WHERE " + m_lfield;
        query += " = '" + id + "'";
    }

    rset = m_db->ExecuteQuery(query);

    m_cur_values[m_rfield] = rset.GetColumn(m_rfield);
}

// Tambem esta levando em consideracao somente a direita...verificar depois
void BWDbRelation::Clean()
{
    m_new_values[m_rfield].clear();
    m_new_values[m_rfield].push_back("<%!EMPTY!%>");
}


/*
 * TODO: Isto aqui esta por fazer ainda...vamos ver depois com calma
 *
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
*/

